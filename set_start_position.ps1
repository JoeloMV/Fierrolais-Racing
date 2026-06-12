Add-Type -AssemblyName System.Drawing
$img = [System.Drawing.Bitmap]::FromFile('assets/pista.png')
function IsBlack($c){ return $c.R -lt 100 -and $c.G -lt 100 -and $c.B -lt 100 }
function IsWhite($c){ return $c.R -gt 220 -and $c.G -gt 220 -and $c.B -gt 220 }
for ($y=58; $y -le 72; $y++) {
    $line = ("y={0}: " -f $y)
    for ($x=98; $x -le 140; $x++) {
        $c = $img.GetPixel($x, $y)
        if (IsBlack($c)) { $line += 'B' }
        elseif (IsWhite($c)) { $line += 'W' }
        else { $line += '.' }
    }
    Write-Host $line
}
$img.Dispose()
