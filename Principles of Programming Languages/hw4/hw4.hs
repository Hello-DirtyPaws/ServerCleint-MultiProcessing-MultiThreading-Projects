import System.Random


-- ****************** GET A WORD FROM THE LIST ***********************

-- generates a list of words in IO monad form
wrdList :: String -> IO [String]
wrdList str = readFile str >>= (\s -> return (words s))

-- gets the length of the words list generated in IO Int type
getLen :: String -> IO Int
getLen str = do
	strLst <- (wrdList str)
	let n = (length strLst)
	return (n)

-- Return the nth word from the wordList in IO String form
getNWrd :: String -> Int -> IO String
getNWrd str n = do
	len <- (getLen str)
	if(n < 0 || n > (len-1))
	then return ("")
	else (wrdList str >>= (\s -> return (s !! n)))
	
-- ****************** MAKE ALL RUN  ***********************


randNum ::String -> IO Int
randNum str = do
	rand <- newStdGen
	len <- getLen str
	let (x,_) = randomGen len rand
	return(x)	

randomGen :: Int -> StdGen-> (Int,StdGen)
randomGen l rand = randomR (0, l-1) rand

-- ****************** GENERATE [CHAR]  ***********************

charChk :: Char -> [Char] -> Char
charChk c [] = '_'
charChk c (x:xs) = do
	if(c == x)
	then c
	else charChk c xs

outGen :: String -> [Char] -> String
outGen [] _ = ""
outGen (x:xs) usr = (charChk x usr) : (outGen xs usr)

-- ****************** TAKE INPUT  ***********************

getInput :: IO Char
getInput = do 
	putStrLn("Input: ")
	c <- getLine 
	if(length c == 0)
	then getInput
	else return (head c)

-- **************** CHECK IF COMPLETE  ********************

isItDone :: String -> Bool
isItDone [] = True
isItDone (x:xs) = do 
	if(x == '_')
	then False
	else isItDone xs
	
-- ****************** MAKE ALL RUN  ***********************

caller :: String -> [Char] -> Int -> IO()
caller strFile chLst n = do
	c <- getInput
	putStr("\n")
	wrdX <- (getNWrd strFile n)
	let newChLst = c : chLst
	let str = (outGen (wrdX) newChLst)
	putStrLn (show str)
	if(isItDone str)
	then putStrLn("Game Complete!!")
	else (caller strFile newChLst n)
	

hangman :: String -> IO()
hangman str = do
	num <- randNum str
	caller str [] num


main :: IO()
main = hangman wrdTxt



-- Location of the word file to read
wrdTxt :: String
-- wrdTxt = "/Users/surajupadhyay/Desktop/RU/SP-2019/314 - Prin Prog/hw4/words.txt"
wrdTxt = "words.txt"


	