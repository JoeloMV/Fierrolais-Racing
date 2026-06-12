Add-Type -AssemblyName System.Drawing
$img = [System.Drawing.Bitmap]::FromFile('assets/pista.png')
function IsBlack($c){ return $c.R -lt 80 -and $c.G -lt 80 -and $c.B -lt 80 }
function IsWhite($c){ return $c.R -gt 220 -and $c.G -gt 220 -and $c.B -gt 220 }
for ($y=55; $y -le 95; $y += 5) {
    $row = "y={0:D2}: " -f $y
    for ($x=80; $x -le 260; $x += 4) {
        $c = $img.GetPixel($x, $y)
        if (IsBlack($c)) { $row += 'B' }
        elseif (IsWhite($c)) { $row += 'W' }
        else { $row += '.' }
    }
    Write-Host $row
}
$img.Dispose()