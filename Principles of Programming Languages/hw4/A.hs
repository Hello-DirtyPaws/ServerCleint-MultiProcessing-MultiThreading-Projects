import System.Random

main = do
	rand <- newStdGen
	let (x,_) = randomGen 40 rand
	print x

randomGen :: Int -> StdGen-> (Int,StdGen)
randomGen l rand = randomR (0, l-1) rand

