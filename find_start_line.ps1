Add-Type -AssemblyName System.Drawing
$img = [System.Drawing.Bitmap]::FromFile('assets/pista.png')
$w = $img.Width
$h = $img.Height
Write-Host "size $w $h"
function IsBlack($c){ return $c.R -lt 80 -and $c.G -lt 80 -and $c.B -lt 80 }
function IsWhite($c){ return $c.R -gt 220 -and $c.G -gt 220 -and $c.B -gt 220 }
$matches = @()
for ($y=0; $y -lt 200; $y++) {
    for ($x=0; $x -lt 260; $x++) {
        $score = 0
        for ($dy=0; $dy -lt 10; $dy++) {
            for ($dx=0; $dx -lt 10; $dx++) {
                if ($x+$dx -ge $w -or $y+$dy -ge $h) { continue }
                $c = $img.GetPixel($x+$dx, $y+$dy)
                $expectedWhite = (($dx + $dy) % 2) -eq 0
                if ($expectedWhite -and (IsWhite $c)) { $score += 1 }
                if (-not $expectedWhite -and (IsBlack $c)) { $score += 1 }
            }
        }
        if ($score -gt 70) {
            $matches += [PSCustomObject]@{ X=$x; Y=$y; Score=$score }
        }
    }
}
$img.Dispose()
$matches | Sort-Object Score -Descending | Select-Object -First 20 | ForEach-Object { Write-Host "cand $($_.X) $($_.Y) $($_.Score)" }
