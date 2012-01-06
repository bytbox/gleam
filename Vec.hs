{-# LANGUAGE TemplateHaskell, FlexibleInstances, MultiParamTypeClasses, FunctionalDependencies #-}

module Vec
  where

import Language.Haskell.TH

class Vector v where
  zipV :: (Double -> Double -> Double) -> v -> v -> v
  mapV :: (Double -> Double) -> v -> v

class Projection v w | v -> w where
  proj :: v -> w

inv :: Vector v => v -> v
inv = mapV (\x -> (-x))

(+++) :: Vector v => v -> v -> v
(+++) = zipV (+)

tupleType :: [Type] -> Type
tupleType ts = foldl AppT (TupleT (length ts)) ts

vector :: Int -> Q [Dec]
vector n = do
  t <- typ
  vd <- return . InstanceD [] (AppT (ConT $ mkName "Vec.Vector") t) =<< sequence
          [ do -- mapV
              f <- newName "f"
              names <- sequence $ replicate n $ newName "a"
              let tupP = TupP $ map VarP names
              let tupE = TupE $ map (AppE (VarE f) . VarE) names
              return $ fun "mapV" [VarP f, tupP] tupE
          , do -- zipV
              f <- newName "f"
              namesA <- sequence $ replicate n $ newName "a"
              namesB <- sequence $ replicate n $ newName "b"
              let pa = TupP $ map VarP namesA
              let pb  = TupP $ map VarP namesB
              let ret = TupE $ map (\(a, b) -> (AppE (AppE (VarE f) (VarE a)) (VarE b)))
                          $ zip namesA namesB
              return $ fun "zipV" [VarP f, pa, pb] ret
          ]
  return $ [vd]
  where
    typ :: Q Type
    typ = return . tupleType . replicate n =<< [t| Double |]
    fun :: String -> [Pat] -> Exp -> Dec
    fun n p e = FunD (mkName n) [(Clause p (NormalB e) [])]

vectors :: (Int, Int) -> Q [Dec]
vectors (a, b) = fmap concat $ sequence $ [vector n | n <- [a..b]]
