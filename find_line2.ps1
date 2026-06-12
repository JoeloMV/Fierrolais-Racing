Add-Type -AssemblyName System.Drawing
$img = [System.Drawing.Bitmap]::FromFile('assets/pista.png')
$w = $img.Width
$h = $img.Height
Write-Host "size $w $h"
function IsDark($c){ return $c.R -lt 80 -and $c.G -lt 80 -and $c.B -lt 80 }
function IsLight($c){ return $c.R -gt 220 -and $c.G -gt 220 -and $c.B -gt 220 }
$rows = @()
for ($y=0; $y -lt 180; $y++) {
    $rowScore = 0
    for ($x=0; $x -lt 260; $x++) {
        $c = $img.GetPixel($x, $y)
        if (IsDark($c) -or IsLight($c)) { $rowScore++ }
    }
    if ($rowScore -gt 40) { $rows += [PSCustomObject]@{Y=$y; Score=$rowScore} }
}
$rowCandidates = $rows | Sort-Object Score -Descending | Select-Object -First 20
$rowCandidates | ForEach-Object { Write-Host "row $_.Y score $_.Score" }

$cols = @()
for ($x=0; $x -lt 260; $x++) {
    $colScore = 0
    for ($y=0; $y -lt 180; $y++) {
        $c = $img.GetPixel($x, $y)
        if (IsDark($c) -or IsLight($c)) { $colScore++ }
    }
    if ($colScore -gt 40) { $cols += [PSCustomObject]@{X=$x; Score=$colScore} }
}
$colCandidates = $cols | Sort-Object Score -Descending | Select-Object -First 20
$colCandidates | ForEach-Object { Write-Host "col $_.X score $_.Score" }

# Compute exact bounding box of dark/light dense cluster in that region
$minx=9999; $miny=9999; $maxx=0; $maxy=0
for ($y=0; $y -lt 180; $y++) {
    for ($x=0; $x -lt 260; $x++) {
        $c = $img.GetPixel($x, $y)
        if (IsDark($c) -or IsLight($c)) {
            if ($x -lt $minx) { $minx=$x }
            if ($x -gt $maxx) { $maxx=$x }
            if ($y -lt $miny) { $miny=$y }
            if ($y -gt $maxy) { $maxy=$y }
        }
    }
}
Write-Host "bbox $minx $miny $maxx $maxy width $([math]::Max(0,$maxx-$minx+1)) height $([math]::Max(0,$maxy-$miny+1))"
$img.Dispose()
