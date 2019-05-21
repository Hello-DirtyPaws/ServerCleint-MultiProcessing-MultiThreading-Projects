#lang racket

; *********************************************
; *  314 Principles of Programming Languages  *
; *  Spring 2019                              *
; *  Student Version                          *
; *  SURAJ UPADHYAY                           *
; *  RUID# 189006555                          *
; *********************************************

;; contains "ctv", "A", and "reduce" definitions
(require "include.rkt")

;; contains simple dictionary definition
(require "dictionary.rkt")

;; -----------------------------------------------------
;; HELPER FUNCTIONS

(define funct-gen-checker
  (lambda (dict)
    (lambda (hash)
    (map hash dict)
    )))

(define flatten
  (lambda (l)
    (cond ((null? l) '())
          ((atom? (car l)) (cons (car l) (flatten (cdr l))))
          (else (append (flatten (car l)) (flatten (cdr l)))))))

(define atom?
  (lambda (x)
    (not (pair? x))))

(define hash-word
  (lambda (hsfl word)
    (map (lambda (h) (h word) ) hsfl)
    ))

(define contains?
  (lambda (l i)
     (if (empty? l) #f
          (if (= (car l) i) #t
          (contains? (cdr l) i)))))

(define check
  (lambda (dict-bit word-dict)
    (map (call-contain dict-bit) word-dict)))

(define call-contain
  (lambda (dict-bit)
    (lambda (word-dict)
      (contains? dict-bit word-dict))))

(define key_funct
  (lambda (x y)
    (if (null? x)
        (y)
        (+ (* 29 y) (ctv x)))))

;; -----------------------------------------------------
;; KEY FUNCTION

(define key
  (lambda (w)
    (reduce key_funct w 5413)))

;; -----------------------------------------------------
;; HASH FUNCTION GENERATORS

;; value of parameter "size" should be a prime number
(define gen-hash-division-method
  (lambda (size)
    (lambda (word)
      (modulo (key word) size)
    )
))

(define gen-hash-multiplication-method
  (lambda (size)
    (lambda (w)
      (floor (* (- (* (key w) A) (floor (* (key w) A))) size))
    )
))

;; -----------------------------------------------------
;; EXAMPLE HASH FUNCTIONS AND HASH FUNCTION LISTS

;;(define hash-1 (gen-hash-division-method 70111))
;;(define hash-2 (gen-hash-division-method 89989))
;;(define hash-3 (gen-hash-multiplication-method 700426))
;;(define hash-4 (gen-hash-multiplication-method 952))

;;(define hashfl-1 (list hash-1 hash-2 hash-3 hash-4))
;;(define hashfl-2 (list hash-1 hash-3))
;;(define hashfl-3 (list hash-2 hash-3))

;; -----------------------------------------------------
;; SPELL CHECKER GENERATOR

(define gen-checker 
  (lambda (hashfunctionlist dict)
    (lambda (word)
      (let* ((dict-bit (flatten (map (funct-gen-checker dict) hashfunctionlist)))
             (word-bit (hash-word hashfunctionlist word))
             (final-list (check dict-bit word-bit)))
        (if (member #f final-list) #f #t)
      ))
))

;; -----------------------------------------------------
;; EXAMPLE SPELL CHECKERS

;;(define checker-1 (gen-checker hashfl-1 dictionary))
;;(define checker-2 (gen-checker hashfl-2 dictionary))
;;(define checker-3 (gen-checker hashfl-3 dictionary))

;; -----------------------------------------------------
;; EXAMPLE APPLICATIONS OF A SPELL CHECKER

;;(checker-1 '(a r g g g g)) ; ==> #f
;;(checker-2 '(h e l l o))   ; ==> #t
;;(checker-2 '(a r g g g g)) ; ==> #f
;;(checker-3 '(b a c k))     ; ==> #t
;;(checker-2 '(w o r d))     ; ==> #t
;;(checker-3 '(s u r a j))   ; ==> #f
;;(checker-1 '(a b c d e f g h i j k l m n o p q r s t u v w x y z a b c d e f g h i j k l m n o p q r s t u v))

