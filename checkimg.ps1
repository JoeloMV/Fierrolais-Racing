Add-Type -AssemblyName System.Drawing
$img = [System.Drawing.Bitmap]::FromFile('assets\images\fierrarif8.png')
$w = $img.Width
$h = $img.Height
Write-Host "Size: $w $h"
$cols = @()
for ($x = 0; $x -lt $w; $x++) {
    $has = $false
    for ($y = 0; $y -lt $h; $y++) {
        $c = $img.GetPixel($x, $y)
        if ($c.R -ne 255 -or $c.G -ne 255 -or $c.B -ne 255) {
            $has = $true
            break
        }
    }
    $cols += $has
}
$rows = @()
for ($y = 0; $y -lt $h; $y++) {
    $has = $false
    for ($x = 0; $x -lt $w; $x++) {
        $c = $img.GetPixel($x, $y)
        if ($c.R -ne 255 -or $c.G -ne 255 -or $c.B -ne 255) {
            $has = $true
            break
        }
    }
    $rows += $has
}
function GetBlocks([bool[]]$arr) {
    $blocks = @()
    $in = $false
    $start = 0
    for ($i = 0; $i -lt $arr.Count; $i++) {
        if ($arr[$i] -and -not $in) {
            $in = $true
            $start = $i
        } elseif (-not $arr[$i] -and $in) {
            $blocks += [PSCustomObject]@{ Start = $start; End = $i - 1 }
            $in = $false
        }
    }
    if ($in) {
        $blocks += [PSCustomObject]@{ Start = $start; End = $arr.Count - 1 }
    }
    return $blocks
}
$bx = GetBlocks $cols
$by = GetBlocks $rows
Write-Host "xblocks: $($bx | ForEach-Object { $_.Start + '-' + $_.End } | Out-String)"
Write-Host "yblocks: $($by | ForEach-Object { $_.Start + '-' + $_.End } | Out-String)"
$firstx = $bx[0].Start
$firstxEnd = $bx[0].End
$firsty = $by[0].Start
$firstyEnd = $by[0].End
$minx = $w
$miny = $h
$maxx = 0
$maxy = 0
for ($y = $firsty; $y -le $firstyEnd; $y++) {
    for ($x = $firstx; $x -le $firstxEnd; $x++) {
        $c = $img.GetPixel($x, $y)
        if ($c.R -ne 255 -or $c.G -ne 255 -or $c.B -ne 255) {
            if ($x -lt $minx) { $minx = $x }
            if ($x -gt $maxx) { $maxx = $x }
            if ($y -lt $miny) { $miny = $y }
            if ($y -gt $maxy) { $maxy = $y }
        }
    }
}
Write-Host "first: $minx $miny $maxx $maxy $([math]::Max(0, $maxx - $minx + 1)) $([math]::Max(0, $maxy - $miny + 1))"
$img.Dispose()
