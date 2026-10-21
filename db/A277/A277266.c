/*
 * A277266  The number of Fibonacci type sequences which contain n after the
 *          initial terms.
 * https://oeis.org/A277266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 142, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A277266: number of Fibonacci‑type sequences that contain n
 * after the initial two terms.
 *
 * For a Fibonacci‑type sequence defined by a0=x, a1=y (x,y≥0)
 * a_m = F_{m‑1}·x + F_m·y  (m≥2), where F_1=F_2=1, F_{k}=F_{k‑1}+F_{k‑2}.
 *
 * The term n appears in the sequence iff there is an m≥2 such that
 * n = F_{m‑1}·x + F_m·y.
 * Since consecutive Fibonacci numbers are coprime, for each m the
 * linear Diophantine equation has a finite number of non‑negative
 * solutions.  We sum these counts over all m with F_{m‑1}≤n.
 *
 * The pair (n,0) is counted twice (for m=2 and m=3); we subtract one
 * for n>0 to obtain the correct answer.
 */

#include <stdint.h>
#include <stddef.h>

/* Extended Euclid to compute modular inverse modulo m (gcd(a,m)=1) */
static uint64_t modinv_uint64(uint64_t a, uint64_t m) {
    __int128 t = 0, newt = 1;
    __int128 r = m, newr = a;
    while (newr != 0) {
        uint64_t q = (uint64_t)(r / newr);
        __int128 tmp_r = r - (__int128)q * newr;
        r = newr; newr = tmp_r;
        __int128 tmp_t = t - (__int128)q * newt;
        t = newt; newt = tmp_t;
    }
    /* r is gcd, must be 1 for invertible a modulo m */
    if (r != 1) return 0;
    if (t < 0) t += (__int128)m;
    return (uint64_t)t;
}

seq_t a(idx_t n) {
    /* Fibonacci numbers up to > n (F1=1,F2=1) */
    uint64_t fib[95];          /* Fibonacci numbers fit in 64 bits for n≤2^64-1 */
    fib[1] = 1;
    fib[2] = 1;
    size_t k = 2;
    while (fib[k] <= (uint64_t)n) {
        ++k;
        fib[k] = fib[k-1] + fib[k-2];
    }
    /* Count solutions for each m with F_{m-1} ≤ n */
    uint64_t total = 0;
    for (size_t m = 2; m <= k; ++m) {
        uint64_t f1 = fib[m-1];   /* coefficient of x   */
        uint64_t f2 = fib[m];     /* coefficient of y   */
        uint64_t y_max = n / f2;  /* maximum y with f2*y ≤ n */
        uint64_t ways = 0;

        if (f1 == 1) {
            /* every y from 0 to y_max works */
            ways = y_max + 1;
        } else {
            uint64_t a_mod = f2 % f1;
            uint64_t inv = modinv_uint64(a_mod, f1);      /* inverse of f2 modulo f1 */
            uint64_t rem = n % f1;
            unsigned __int128 prod = (unsigned __int128)rem * inv;
            uint64_t y0 = (uint64_t)(prod % f1);
            if (y0 <= y_max) {
                ways = ((y_max - y0) / f1) + 1;
            }
        }
        total += ways;
    }
    /* The pair (n,0) appears twice (for m=2 and m=3), remove one copy */
    if (n > 0) --total;
    return (seq_t)total;
}
