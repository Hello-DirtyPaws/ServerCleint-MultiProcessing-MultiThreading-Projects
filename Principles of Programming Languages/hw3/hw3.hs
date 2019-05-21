{-****************************** Hopscotch ******************************-}

skips :: (Ord z) => [z] -> [[z]]
skips list = do
 caller list 1

caller :: (Ord z) => [z] -> Int -> [[z]]
caller list p = do
 if(p > length list)
 then []
 else ([(cutter list (p-1) p)] ++ (caller list (p+1)))


cutter :: (Ord z) => [z] -> Int -> Int -> [z]
cutter list p q = do
 if(p >= length list)
 then []
 else (list !! p) : (cutter list (p+q) q)

{-****************************** \Hopscotch ******************************-}



{-***************************** Local maxima *****************************-}

localMaxima :: [Integer] -> [Integer]
localMaxima [] = []
localMaxima (x:[]) = []
localMaxima (x: y: []) = [] 
localMaxima (x: y: z: xs) = do
 if(x < y)
 then 
	if(z < y)
	then y : (localMaxima (y : z : xs))
	else (localMaxima (y : z : xs))
 else (localMaxima (y : z : xs))

{-***************************** \Local maxima *****************************-}



{-******************************* Histogram *******************************-}

histogram :: [Integer] -> String
histogram intList = ((strGen (getFreqList x intList) "") ++ baseString)

baseString :: String
baseString = "==========\n0123456789\n"

x :: [Integer]
x = [0,0,0,0,0,0,0,0,0,0]

-- CREATES A FREQ. LIST THAT GIVES YOU THE #OF TIMES A VALUE APPEARED IN THE GIVEN LIST,
-- USING THE LOCAL LIST
getFreqList :: [Integer] -> [Integer] -> [Integer]
getFreqList locList givenList = do
	if(length givenList == 0)
	then locList
	else
		let a = changeListVal locList (fromIntegral(head givenList)) ((locList !! (fromIntegral(head givenList))) + 1)
			in getFreqList a (tail givenList)
		
-- CHANGES AN ELEMENT IN THE GIVEN LIST AT GIVEN POSITION
changeListVal :: [Integer] -> Int -> Integer -> [Integer]
changeListVal [] _ _ = []
changeListVal (x:xs) loc val = do
	if (length (x:xs) <= loc)
	then (x:xs)
	else
		if(loc == 0)
		then (val:xs)
		else (x: (changeListVal xs (loc-1) val))

strGen :: [Integer] -> String -> String
strGen [] _ = ""
strGen freqList str = do
	if(not (checkAllZero freqList))
	then 
		 let a = (lineGen (freqList) ((""), []))
		 in (strGen (snd a) ((fst a) ++ str))
	else str

checkAllZero :: [Integer] -> Bool
checkAllZero [] = False
checkAllZero [0] = True
checkAllZero (x:xs) = do
	if(x /= 0)
	then False
	else (checkAllZero xs)
	
--should be called on with -> lineGen freqList ("", [])
--   provided list, (provided str with new list to add in), (updated str, updated newList)
lineGen :: [Integer] -> (String, [Integer]) -> (String, [Integer])
lineGen [] (str, y)= ((str ++ "\n"), y)
lineGen (x:xs) (str, newList) = do
	if (x > 0)
	then (lineGen xs ((str ++ "*"), (newList ++ [(x-1)])))
	else (lineGen xs ((str ++ " "), (newList ++ [x])))


{-******************************* \Histogram *******************************-}