#lang racket
(require "include.rkt")

(define key
  (lambda (w)
    (reduce key_funct w 5413)))

(define key_funct
  (lambda (x y)
    (if (null? x)
        (y)
        (+ (* 29 y) (ctv x)))))

(define gen-hash-division-method
  (lambda (size)
    (lambda (word)
      (modulo (key word) size)
    )
))

(define hash-1 (gen-hash-division-method 70111))

(hash-1 '(h e l l o))
(hash-1 '(m a y))
(hash-1 '(t r e e f r o g))