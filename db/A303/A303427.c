/*
 * A303427  Interleaved Lucas and Fibonacci numbers.
 * https://oeis.org/A303427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-4) Recurrence equivalent: a(2k) = Lucas(k)
 *            (Lucas(0)=2), a(2k+1) = Fibonacci(k)
 * Range:     u8 n <= 23, u16 n <= 47, u32 n <= 93, u64 n <= 185,
 *            u128 n <= 369, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: fast‑doubling Fibonacci pair implementation.
   Computes (F_n, F_{n+1}) using only integer arithmetic. */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    seq_t a = 0;   /* F_0 */
    seq_t b = 1;   /* F_1 */

    if (n == 0) {
        *fn  = a;
        *fn1 = b;
        return;
    }

    /* Find most significant bit of n. Use GCC/Clang builtin for speed. */
    idx_t msb = 63 - __builtin_clzll(n);

    for (idx_t i = msb; ; --i) {
        seq_t t1 = a * ((b << 1) - a);   /* a * (2*b – a)   */
        seq_t t2 = a * a + b * b;        /* a*a + b*b       */

        if ((n >> i) & 1U) {
            a = t2;            /* F_{2k+1}   */
            b = t1 + t2;       /* F_{2k+2}   */
        } else {
            a = t1;            /* F_{2k}     */
            b = t2;            /* F_{2k+1}   */
        }

        if (i == 0) break;
    }

    *fn  = a;   /* F_n   */
    *fn1 = b;   /* F_{n+1} */
}

/* The required sequence function. */
seq_t a(idx_t n)
{
    /* a(0) = 2 (Lucas(0)) */
    if (n == 0)
        return (seq_t)2;

    if ((n & 1U) == 0) {                /* even n -> Lucas(n/2) */
        idx_t k = n >> 1;
        /* For k = 0 this branch is not reached because n == 0 was handled. */
        seq_t fk, fk1;
        fib_pair(k, &fk, &fk1);         /* fk = F_k, fk1 = F_{k+1} */
        seq_t fkm1 = fk1 - fk;          /* F_{k-1} = F_{k+1} - F_k  */
        return fkm1 + fk1;              /* Lucas(k) = F_{k-1} + F_{k+1} */
    } else {                            /* odd n -> Fibonacci((n-1)/2) */
        idx_t k = n >> 1;
        seq_t fk, fk1;
        fib_pair(k, &fk, &fk1);         /* fk = F_k */
        return fk;                      /* F_k equals Fibonacci(k) */
    }
}
