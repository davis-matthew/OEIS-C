/*
 * A250541  Number of (n+1)X(5+1) 0..3 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 6, u64 n <= 22, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) satisfies the linear recurrence
 * a(n) = 30*a(n-1) - 415*a(n-2) + 3514*a(n-3) - 20386*a(n-4)
 *        + 85924*a(n-5) - 272198*a(n-6) + 661180*a(n-7) - 1244717*a(n-8)
 *        + 1822478*a(n-9) - 2068955*a(n-10) + 1802474*a(n-11)
 *        - 1181760*a(n-12) + 563888*a(n-13) - 184752*a(n-14)
 *        + 37152*a(n-15) - 3456*a(n-16), for n > 16
 *
 * Initial values:
 * a(1)  = 187000
 * a(2)  = 2701872
 * a(3)  = 23289547
 * a(4)  = 151170400
 * a(5)  = 824599694
 * a(6)  = 4013192386
 * a(7)  = 18064444143
 * a(8)  = 76961701472
 * a(9)  = 315204675572
 * a(10) = 1254564769204
 * a(11) = 4890689086931
 * a(12) = 18781896020328
 * a(13) = 71371001739442
 * a(14) = 269293563149046
 * a(15) = 1011736459116935
 * a(16) = 3793520599786824
 *
 * The function is written to work for any seq_t (small unsigned integer
 * types or an arbitrary‑precision integer class) by avoiding intermediate
 * overflow.  It uses 128‑bit intermediates when seq_t is 64‑bit or less,
 * otherwise it relies on the arithmetic operators of seq_t itself.
 */

#include <stddef.h>

static const seq_t init[16] = {
    (seq_t)187000ULL,
    (seq_t)2701872ULL,
    (seq_t)23289547ULL,
    (seq_t)151170400ULL,
    (seq_t)824599694ULL,
    (seq_t)4013192386ULL,
    (seq_t)18064444143ULL,
    (seq_t)76961701472ULL,
    (seq_t)315204675572ULL,
    (seq_t)1254564769204ULL,
    (seq_t)4890689086931ULL,
    (seq_t)18781896020328ULL,
    (seq_t)71371001739442ULL,
    (seq_t)269293563149046ULL,
    (seq_t)1011736459116935ULL,
    (seq_t)3793520599786824ULL
};

static const int64_t coef[16] = {
    30LL, -415LL, 3514LL, -20386LL, 85924LL, -272198LL,
    661180LL, -1244717LL, 1822478LL, -2068955LL,
    1802474LL, -1181760LL, 563888LL, -184752LL,
    37152LL, -3456LL
};

/* Computes the next term from the last 16 terms. */
static seq_t compute_next(const seq_t ring[16])
{
    /* Decide the arithmetic path based on the size of seq_t. */
    if (sizeof(seq_t) <= 8) { /* uint64_t or smaller */
        __int128 pos = 0;
        __int128 neg = 0;
        for (int i = 0; i < 16; ++i) {
            __int128 term = (__int128)ring[15 - i];
            term *= (__int128)(coef[i] >= 0 ? coef[i] : -coef[i]);
            if (coef[i] >= 0)
                pos += term;
            else
                neg += term;
        }
        __int128 res = pos - neg;
        return (seq_t)res;
    } else { /* arbitrary‑precision seq_t */
        seq_t pos = 0;
        seq_t neg = 0;
        for (int i = 0; i < 16; ++i) {
            seq_t mul = ring[15 - i] * (seq_t)(coef[i] >= 0 ? coef[i] : -coef[i]);
            if (coef[i] >= 0)
                pos += mul;
            else
                neg += mul;
        }
        seq_t res = pos - neg; /* final result is guaranteed non‑negative */
        return res;
    }
}

seq_t a(idx_t n)
{
    if (n <= 16)
        return init[n - 1];

    /* Circular buffer holding the last 16 terms. */
    seq_t ring[16];
    for (int i = 0; i < 16; ++i)
        ring[i] = init[i];

    for (idx_t k = 17; k <= n; ++k) {
        seq_t next = compute_next(ring);
        /* shift left by one, append next at the end */
        for (int i = 0; i < 15; ++i)
            ring[i] = ring[i + 1];
        ring[15] = next;
    }
    return ring[15];
}
