Add-Type -AssemblyName System.Drawing
$img = [System.Drawing.Bitmap]::FromFile('assets/pista.png')
$w = $img.Width
$h = $img.Height
Write-Host "size $w $h"
function IsBlack($c){ return $c.R -lt 80 -and $c.G -lt 80 -and $c.B -lt 80 }
function IsWhite($c){ return $c.R -gt 220 -and $c.G -gt 220 -and $c.B -gt 220 }
for ($y=0; $y -lt 120; $y+=2) {
    $count = 0
    for ($x=0; $x -lt 260; $x++) {
        $c = $img.GetPixel($x,$y)
        if (IsBlack($c) -or IsWhite($c)) { $count++ }
    }
    if ($count -gt 80) { Write-Host "row $y count $count" }
}
for ($x=0; $x -lt 260; $x+=2) {
    $count = 0
    for ($y=0; $y -lt 120; $y++) {
        $c = $img.GetPixel($x,$y)
        if (IsBlack($c) -or IsWhite($c)) { $count++ }
    }
    if ($count -gt 80) { Write-Host "col $x count $count" }
}
$img.Dispose()
