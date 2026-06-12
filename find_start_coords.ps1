Add-Type -AssemblyName System.Drawing
$img = [System.Drawing.Bitmap]::FromFile('assets/pista.png')
$w = $img.Width
$h = $img.Height
Write-Host ("size {0} {1}" -f $w, $h)
function IsBlack($c){ return $c.R -lt 80 -and $c.G -lt 80 -and $c.B -lt 80 }
function IsWhite($c){ return $c.R -gt 220 -and $c.G -gt 220 -and $c.B -gt 220 }
$results = @()
for ($x=10; $x -lt 260; $x++) {
    for ($y=10; $y -lt 140; $y++) {
        $score = 0
        $samples = 0
        for ($dy=0; $dy -lt 16; $dy += 2) {
            $coordY = $y + $dy
            if ($coordY -ge $h) { break }
            $c = $img.GetPixel($x, $coordY)
            if (IsBlack($c) -or IsWhite($c)) {
                $samples++
                if (($coordY % 2) -eq 0) {
                    if (IsWhite($c)) { $score += 1 }
                } else {
                    if (IsBlack($c)) { $score += 1 }
                }
            }
        }
        if ($samples -gt 10 -and $score -gt 8) {
            $results += [PSCustomObject]@{ X=$x; Y=$y; Score=$score; Samples=$samples }
        }
    }
}
$results | Sort-Object -Property Score -Descending | Select-Object -First 20 | ForEach-Object { Write-Host ("cand {0} {1} score {2} samples {3}" -f $_.X, $_.Y, $_.Score, $_.Samples) }
$img.Dispose()
