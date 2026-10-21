/*
 * A245196  Write n>=1 as either n=2^k-2^r with 0 <= r <= k-1, in which case
 *          a(2^k-2^r)=wt(k-r-1), or as n=2^k-2^r+j with 2 <= r <= k-1, 1 <= j
 *          < 2^r-1, in which case a(2^k-2^r+j)=a(j)*wt(k-r-1) (where wt(i) =
 *          A000120(i)).
 * https://oeis.org/A245196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A245196:
   Let k = floor(log2(n)) + 1 and Δ = 2^k – n.
   • If Δ is a power of two, say Δ = 2^r, then
         a(n) = wt(k‑r‑1)      (wt(i) = binary weight of i).
   • Otherwise let r = floor(log2(Δ)) + 1 and
         j = n – (2^k – 2^r).
     Then a(n) = a(j) * wt(k‑r‑1).
   The initial values are a(1) = 0, a(2) = 0, a(3) = 1, … */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* largest exponent e such that 2^e ≤ x */
static inline idx_t floor_log2_idx(idx_t x)
{
    idx_t e = 0;
    while (x > 1)
    {
        x >>= 1;
        ++e;
    }
    return e;
}

/* binary weight (population count) of a idx_t value */
static inline uint64_t popcnt_idx(idx_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return (uint64_t)__builtin_popcountll((unsigned long long)x);
#else
    uint64_t c = 0;
    while (x)
    {
        x &= x - 1;
        ++c;
    }
    return c;
#endif
}

seq_t a(idx_t n)
{
    /* Handle the very small cases directly */
    if (n <= 2)
        return (seq_t)0;

    idx_t floor_log2_n = floor_log2_idx(n);   /* floor(log2(n)) */
    idx_t k = floor_log2_n + 1;               /* k = floor(log2(n))+1 */

    idx_t pow2k = ((idx_t)1) << k;            /* 2^k  (k ≤ 63 for all feasible n) */
    idx_t delta = pow2k - n;                  /* 1 ≤ delta < 2^k */

    if ((delta & (delta - 1)) == 0)           /* delta is a power of two */
    {
        idx_t r = floor_log2_idx(delta);      /* r with delta = 2^r */
        idx_t i = k - r - 1;                  /* i ≥ 0 */
        return (seq_t)popcnt_idx(i);
    }
    else                                     /* delta is not a power of two */
    {
        idx_t r = floor_log2_idx(delta) + 1;  /* smallest r such that 2^r > delta */
        idx_t i = k - r - 1;
        seq_t weight = (seq_t)popcnt_idx(i);

        idx_t base = pow2k - (((idx_t)1) << r);
        idx_t j = n - base;                  /* 1 ≤ j < 2^r – 1 */

        seq_t aj = a(j);
        return aj * weight;
    }
}
