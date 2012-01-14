> {-# LANGUAGE TemplateHaskell #-}

> module Main
>   where

> import PNG (writePNG)
> import Vectors

Computation is done from destination to source; each ray is traced from the
destination pixel to each possible source, forking as necessary along the way
via weighted summations.

> data Ray = Ray
>   { frequencies :: [(Double, Double)]
>   , destination :: $(vec 3)
>   , direction :: $(vec 3)
>   }

For display, we consider color in the traditional RGB values, rather than as a
frequency.

> type Color = (Double, Double, Double)

> blank :: Int -> Int -> [[Color]]
> blank w h = replicate w $ replicate h (0, 0, 0)

The result of our raytracing is represented as a matrix of absolute colors,
which must be normalized into something displayable.

> normalize :: [[Color]] -> [[(Int, Int, Int)]]
> normalize cs = map (map $ map3t normalizeComponent) cs
>   where
>     normalizeComponent c = cap 255 $ round (c * 256)
>     cap m v = if m > v then v else m

> main = writePNG "out.png" $ normalize $ blank 10 10

> map3t f (a, b, c) = (f a, f b, f c)
