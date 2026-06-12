Add-Type -AssemblyName System.Drawing
$img = [System.Drawing.Bitmap]::FromFile('assets/pista.png')
$w = $img.Width
$h = $img.Height
function IsBlack($c){ return $c.R -lt 80 -and $c.G -lt 80 -and $c.B -lt 80 }
function IsWhite($c){ return $c.R -gt 220 -and $c.G -gt 220 -and $c.B -gt 220 }
Write-Host "size $w $h"
for ($y = 50; $y -le 80; $y++) {
    $rowCount = 0
    for ($x = 80; $x -le 250; $x++) {
        $c = $img.GetPixel($x, $y)
        if (IsBlack($c) -or IsWhite($c)) { $rowCount++ }
    }
    if ($rowCount -gt 50) { Write-Host "row $y count $rowCount" }
}
Write-Host '---'
for ($y = 58; $y -le 70; $y++) {
    $prefix = ("y={0:D2}: " -f $y)
    for ($x = 90; $x -le 160; $x += 2) {
        $c = $img.GetPixel($x, $y)
        if (IsBlack($c)) { $prefix += 'B' }
        elseif (IsWhite($c)) { $prefix += 'W' }
        else { $prefix += '.' }
    }
    Write-Host $prefix
}
$img.Dispose()
