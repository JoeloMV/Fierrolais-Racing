Add-Type -AssemblyName System.Drawing
$img = [System.Drawing.Bitmap]::FromFile('assets/pista.png')
for ($y=50; $y -le 90; $y++) {
    $line = "y=$y: "
    for ($x=80; $x -le 240; $x++) {
        $c = $img.GetPixel($x, $y)
        if ($c.R -lt 80 -and $c.G -lt 80 -and $c.B -lt 80) { $line += 'B' }
        elseif ($c.R -gt 220 -and $c.G -gt 220 -and $c.B -gt 220) { $line += 'W' }
        else { $line += '.' }
    }
    Write-Host $line
}
$img.Dispose()
