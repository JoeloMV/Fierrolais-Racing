Add-Type -AssemblyName System.Drawing
$img = [System.Drawing.Bitmap]::FromFile('assets/pista.png')
$w = $img.Width
$h = $img.Height
Write-Host "size $w $h"
function IsBlack($c){ return $c.R -lt 60 -and $c.G -lt 60 -and $c.B -lt 60 }
function IsWhite($c){ return $c.R -gt 220 -and $c.G -gt 220 -and $c.B -gt 220 }
for ($y=10; $y -le 60; $y += 2) {
    $line = "y=$y:"
    for ($x=100; $x -le 360; $x += 4) {
        $c = $img.GetPixel($x, $y)
        if (IsBlack($c)) { $line += 'B' }
        elseif (IsWhite($c)) { $line += 'W' }
        else { $line += '.' }
    }
    Write-Host $line
}
$img.Dispose()