Add-Type -AssemblyName System.Drawing
$img = [System.Drawing.Bitmap]::FromFile('assets/pista.png')
function IsBlack($c){ return $c.R -lt 100 -and $c.G -lt 100 -and $c.B -lt 100 }
function IsWhite($c){ return $c.R -gt 220 -and $c.G -gt 220 -and $c.B -gt 220 }
Write-Host "size $($img.Width) $($img.Height)"
for ($y=45; $y -le 80; $y++) {
    for ($x=80; $x -le 230; $x+=4) {
        $trans=0
        $valid=0
        $prev=''
        for ($dx=0; $dx -lt 50; $dx++) {
            $xx=$x+$dx
            if ($xx -ge $img.Width) { break }
            $c=$img.GetPixel($xx,$y)
            $cur='.'
            if (IsWhite($c)) { $cur='W' }
            elseif (IsBlack($c)) { $cur='B' }
            if ($cur -ne '.') {
                $valid++
                if ($prev -ne '' -and $cur -ne $prev) { $trans++ }
                $prev=$cur
            }
        }
        if ($valid -gt 25 -and $trans -gt 10) {
            Write-Host "cand x=$x y=$y trans=$trans valid=$valid"
        }
    }
}
$img.Dispose()
