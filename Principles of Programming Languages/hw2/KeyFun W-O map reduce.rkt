#lang racket

(define reduce
  (lambda (op l id)
    (if (null? l)
        id
        (op (car l) (reduce op (cdr l) id)))))


(define key
  (lambda (l)
    (if(null? l) 5413
       (+ (* 29 (key (cdr l))) (car l)))))

(require racket/trace)
(trace key)

(key '(8 5 12 12 15))
