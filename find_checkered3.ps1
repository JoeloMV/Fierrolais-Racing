Add-Type -AssemblyName System.Drawing
$img = [System.Drawing.Bitmap]::FromFile('assets/pista.png')
$w = $img.Width
$h = $img.Height
function IsBlack($c){ return $c.R -lt 80 -and $c.G -lt 80 -and $c.B -lt 80 }
function IsWhite($c){ return $c.R -gt 220 -and $c.G -gt 220 -and $c.B -gt 220 }
Write-Host "size $w $h"
$best = @()
for ($y=0; $y -lt 120; $y++) {
    for ($x=0; $x -lt 260; $x++) {
        $count = 0
        $prev = ''
        $valid = 0
        for ($dx=0; $dx -lt 24; $dx++) {
            $xx = $x + $dx
            if ($xx -ge $w) { break }
            $c = $img.GetPixel($xx, $y)
            $color = 'N'
            if (IsBlack($c)) { $color = 'B' }
            elseif (IsWhite($c)) { $color = 'W' }
            if ($color -ne 'N') {
                $valid++
                if ($prev -ne '' -and $color -ne $prev) { $count++ }
                $prev = $color
            }
        }
        if ($valid -ge 18 -and $count -ge 8) {
            $best += [PSCustomObject]@{X=$x;Y=$y;Score=$count;Valid=$valid}
        }
    }
}
$best | Sort-Object -Property Score -Descending, Y, X | Select-Object -First 20 | ForEach-Object { Write-Host ("cand {0} {1} score {2} valid {3}" -f $_.X, $_.Y, $_.Score, $_.Valid) }
$img.Dispose()
