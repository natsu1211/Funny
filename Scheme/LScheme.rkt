#lang racket

(define initial-env (list (cons '+ +)(cons '- -)(cons '* *)(cons '/ /)(cons '> >)(cons '< <)(cons '= =)
                    (cons 'cons cons)))

;; extend enviornment 
(define ext-env
  (lambda (x v env)
    (cons `(,x . ,v) env)))

;; find x in enviornment 
(define lookup
  (lambda (x env)
    (let ([p (assq x env)])
      (cond
       [(not p) (error x"undefined;
cannot reference an identifier before its definition\n" )]  
       [else (cdr p)]))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; closure
(struct Closure (f env))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


(define eval1
  (lambda (exp env)
     (match exp                                                                   
       [(? string? exp) exp]                                                         
       [(? number? exp)  exp]                              
       [(? boolean? exp) exp]                              
       [(? symbol? exp) (lookup exp env)]                 
       [`(lambda (,x) ,e)                                 
       (Closure exp env)]
       ;todo
       [`(cond . ,e  )                                     ;;cond
       (let ((e1 (first e))
             (re (rest e)))
        [match e1
          [`(else ,e2)
           (eval1 e2 env)]
          [else (if(eval1 (first e1) env)                 
                  (eval1 (second e1) env)
                  (eval1  (cons `cond re) env))]])]
      [`(and . ,exps )                                     ;;and
          (cond((empty?  exps)#t)
               ((not(eval1 (first exps) env))#f)
               (else(eval1 (cons 'and (rest exps)) env)))]
      [`(or . ,exps )                                      ;;or
          (cond((empty?  exps)#f)
               ((eval1 (first exps) env)#t)
               (else(eval1 (cons 'and (rest exps)) env)))]
      [`(begin  ,exps ...)                                 ;;begin
       (foldl (lambda (e _) (eval1 e env)) empty exps)]                             
      [`(set! ,var ,exps)                                  ;;set!
       (let((x1(assq var env)))(set-cdr! x1 (eval  exps env)))]
      [`(define ,name ,exps)                               ;; define, todo
       (let ([value (eval1 exps env)])
         (set! initial-env (ext-env name value env)))]
       ;;todo
      [`(if ,ec ,et ,ef)    (if (eval1 ec env)             ;;if
                              (eval1 et env)
                              (eval1 ef env))]
       [`(quote ,e)                                        ;;quote
       e]
       [`(,op . ,e1 )                                      ;;call
       (let ([op1 (eval1 op env)]
             [v1  (eval-map e1 env)])
         (match op1
                [(Closure `(lambda (,x) ,e) env1)
                 (eval1 e (ext-env x v1 env1))]
                [else
                 (apply op1 v1)]))])))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;help function
(define (eval-map e1 env)
  (if(= (length e1)1)
     (eval1 (car e1) env)
     (map (lambda(e)(eval1 e env)) e1)))
(define first car)
(define second cadr)
(define rest cdr)
(define-syntax-rule (set-car! lst val)                     ;;set-car!
(if (not (pair? lst))
     lst
     (set! lst (cons val (cdr lst))))) 
(define-syntax-rule (set-cdr! lst val)                     ;;set-cdr!
  (if (not (list? lst))
     lst
  (set! lst (cons(car lst) val))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; initial info  
(define welcome 
       (lambda()
          (display "\n")
          (display "**********************************\n")
          (display "*                                *\n")
          (display "*     Welcome to LScheme         *\n")
          (display "*                                *\n")
          (display "**********************************\n")
          (display "\n") ))
(define start
  (lambda ()
    (call/cc 
     (lambda (k)
       (display ">>>")
       (let ([exp (read)])
         (if (not (equal? 'exit exp))
             (display (eval1 exp initial-env))
             (k 'done)))
       (newline)
     (start)))))
(define (LScheme)
     (welcome)
    (start))
(LScheme)