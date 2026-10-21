/*
 * A251649  Number of (n+2)X(5+2) 0..3 arrays with no row, column, diagonal or
 *          antidiagonal in any 3X3 subblock summing to 0 2 4 5 7 or 9.
 * https://oeis.org/A251649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence for n > 14 a(n) = 6*a(n-1) - 3*a(n-2) +
 *            18*a(n-3) - 234*a(n-4) + 144*a(n-5) + 993*a(n-6) - 756*a(n-7) -
 *            755*a(n-8) + 630*a(n-9) - 42*a(n-11) a(1) = 2382, a(2) = 10248,
 *            a(3) = 31022, a(4) = 98306, a(5) = 482506, a(6) = 1563586, a(7)
 *            = 5248890, a(8) = 25151338, a(9) = 86588602,...
 * Range:     u8 none, u16 n <= 3, u32 n <= 11, u64 n <= 27, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Initial terms a(0) … a(14) – a(0) is unused */
static const seq_t _init_terms[15] = {
    (seq_t)0,          /* a(0) unused, keeps indices 1‑based */
    (seq_t)2382,       /* a(1) */
    (seq_t)10248,      /* a(2) */
    (seq_t)31022,      /* a(3) */
    (seq_t)98306,      /* a(4) */
    (seq_t)482506,     /* a(5) */
    (seq_t)1563586,    /* a(6) */
    (seq_t)5248890,    /* a(7) */
    (seq_t)25151338,   /* a(8) */
    (seq_t)86588602,   /* a(9) */
    (seq_t)309874186,  /* a(10) */
    (seq_t)1436889898, /* a(11) */
    (seq_t)5253854634, /* a(12) */
    (seq_t)19877542282,/* a(13) */
    (seq_t)89455192554 /* a(14) */
};

/* Perform multiplication by a small positive constant.
   Two implementations are provided: one for C (uses __int128),
   the other for C++ (uses shift‑and‑add to avoid overflow). */
#ifdef __cplusplus
static inline seq_t mul_const_cpp(const seq_t &x, unsigned int c)
{
    seq_t res = 0;
    seq_t term = x;
    while (c > 0) {
        if (c & 1U)
            res += term;
        term <<= 1;
        c >>= 1;
    }
    return res;
}
static inline seq_t compute_next(const seq_t *last)
{
    seq_t term = mul_const_cpp(last[0], 6);
    term -= mul_const_cpp(last[1], 3);
    term += mul_const_cpp(last[2], 18);
    term -= mul_const_cpp(last[3], 234);
    term += mul_const_cpp(last[4], 144);
    term += mul_const_cpp(last[5], 993);
    term -= mul_const_cpp(last[6], 756);
    term -= mul_const_cpp(last[7], 755);
    term += mul_const_cpp(last[8], 630);
    term -= mul_const_cpp(last[10], 42);
    return term;
}
#else
/* C implementation – all intermediate arithmetic uses __int128
   to avoid 64‑bit overflow. */
static inline seq_t compute_next(const seq_t *last)
{
    __int128 total = 0;
    total += (__int128)6 * (__int128)last[0];
    total -= (__int128)3 * (__int128)last[1];
    total += (__int128)18 * (__int128)last[2];
    total -= (__int128)234 * (__int128)last[3];
    total += (__int128)144 * (__int128)last[4];
    total += (__int128)993 * (__int128)last[5];
    total -= (__int128)756 * (__int128)last[6];
    total -= (__int128)755 * (__int128)last[7];
    total += (__int128)630 * (__int128)last[8];
    total -= (__int128)42  * (__int128)last[10];
    return (seq_t)total;
}
#endif

/* Public sequence function – external linkage */
seq_t a(idx_t n)
{
    /* Offset handling – a(1) is the first term.  a(0) is unused. */
    if (n == 0u)
        return (seq_t)0;

    if (n <= 14u)
        return _init_terms[(size_t)n];

    /* Buffer for the 11 most recent terms:
       last[0] = a(k-1), last[1] = a(k-2), … , last[10] = a(k-11).  */
    seq_t last[11];
    for (idx_t i = 0u; i < 11u; ++i)
        last[i] = _init_terms[14u - i];

    seq_t term = 0;
    for (idx_t cur = 15u; cur <= n; ++cur) {
        term = compute_next(last);
        /* shift buffer */
        for (idx_t j = 10u; j > 0u; --j)
            last[j] = last[j - 1];
        last[0] = term;
    }
    return term;
}
