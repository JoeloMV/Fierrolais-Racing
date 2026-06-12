Add-Type -AssemblyName System.Drawing
$img = [System.Drawing.Bitmap]::FromFile('assets/pista.png')
$w = $img.Width
$h = $img.Height
Write-Host ("size {0} {1}" -f $w, $h)
function IsBlack($c){ return $c.R -lt 80 -and $c.G -lt 80 -and $c.B -lt 80 }
function IsWhite($c){ return $c.R -gt 220 -and $c.G -gt 220 -and $c.B -gt 220 }
for ($y = 40; $y -le 90; $y += 2) {
    $line = ("y={0}:" -f $y)
    for ($x = 50; $x -le 260; $x += 4) {
        $c = $img.GetPixel($x, $y)
        if (IsBlack($c)) { $line += 'B' }
        elseif (IsWhite($c)) { $line += 'W' }
        else { $line += '.' }
    }
    Write-Host $line
}
$img.Dispose()
