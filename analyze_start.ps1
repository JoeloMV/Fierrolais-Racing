Add-Type -AssemblyName System.Drawing
$img = [System.Drawing.Bitmap]::FromFile('assets/pista.png')
$w = $img.Width
$h = $img.Height
Write-Host "size $w $h"
function IsBlack($c){ return $c.R -lt 80 -and $c.G -lt 80 -and $c.B -lt 80 }
function IsWhite($c){ return $c.R -gt 220 -and $c.G -gt 220 -and $c.B -gt 220 }
$rows = @()
for ($y=0; $y -lt 150; $y++) {
    $blacks = 0
    $whites = 0
    for ($x=0; $x -lt 260; $x++) {
        $c = $img.GetPixel($x, $y)
        if (IsBlack($c)) { $blacks++ }
        elseif (IsWhite($c)) { $whites++ }
    }
    if ($blacks -gt 20 -and $whites -gt 20) { $rows += [PSCustomObject]@{Y=$y; Blacks=$blacks; Whites=$whites} }
}
$rows | Sort-Object Blacks -Descending | Select-Object -First 20 | ForEach-Object { Write-Host ("row {0} blacks {1} whites {2}" -f $_.Y, $_.Blacks, $_.Whites) }

$cols = @()
for ($x=0; $x -lt 260; $x++) {
    $blacks = 0
    $whites = 0
    for ($y=0; $y -lt 150; $y++) {
        $c = $img.GetPixel($x, $y)
        if (IsBlack($c)) { $blacks++ }
        elseif (IsWhite($c)) { $whites++ }
    }
    if ($blacks -gt 20 -and $whites -gt 20) { $cols += [PSCustomObject]@{X=$x; Blacks=$blacks; Whites=$whites} }
}
$cols | Sort-Object Blacks -Descending | Select-Object -First 20 | ForEach-Object { Write-Host ("col {0} blacks {1} whites {2}" -f $_.X, $_.Blacks, $_.Whites) }

$img.Dispose()
