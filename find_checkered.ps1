Add-Type -AssemblyName System.Drawing
$img = [System.Drawing.Bitmap]::FromFile('assets/pista.png')
$w = $img.Width
$h = $img.Height
function IsBlack($c){ return $c.R -lt 80 -and $c.G -lt 80 -and $c.B -lt 80 }
function IsWhite($c){ return $c.R -gt 220 -and $c.G -gt 220 -and $c.B -gt 220 }
Write-Host "size $w $h"
$best = @()
for ($y = 30; $y -le 90; $y++) {
    for ($x = 80; $x -le 260; $x++) {
        $pattern = @()
        for ($dx = 0; $dx -lt 40; $dx += 2) {
            $c = $img.GetPixel($x + $dx, $y)
            if (IsBlack($c)) { $pattern += 'B' }
            elseif (IsWhite($c)) { $pattern += 'W' }
            else { $pattern += '.' }
        }
        $score = 0
        for ($i=1; $i -lt $pattern.Count; $i++) {
            if ($pattern[$i] -ne $pattern[$i-1] -and $pattern[$i] -ne '.' -and $pattern[$i-1] -ne '.') { $score++ }
        }
        if ($score -ge 6) {
            $best += [PSCustomObject]@{X=$x;Y=$y;Score=$score;Pattern=($pattern -join '')}
        }
    }
}
$best | Sort-Object Score -Descending, Y, X | Select-Object -First 20 | ForEach-Object { Write-Host "cand $($_.X) $($_.Y) score $($_.Score) pat $($_.Pattern)" }
$img.Dispose()
