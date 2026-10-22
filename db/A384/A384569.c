/*
 * A384569  Consecutive states of the linear congruential pseudo-random number
 *          generator (3141592221*s+1) mod 10^10 when started at s=1.
 * https://oeis.org/A384569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(n) = (3141592221 * a(n-1) + 1) mod 10^10 for n > 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#ifdef __cplusplus
#include <type_traits>
#endif

/* Generic modular multiplication for built‑in or arbitrary‑precision types. */
static inline seq_t mulmod(seq_t a, seq_t b, seq_t mod)
{
#ifdef __cplusplus
    if constexpr (std::is_same_v<seq_t, uint64_t>) {
        return (seq_t)((unsigned __int128)a * b % mod);
    } else if constexpr (std::is_same_v<seq_t, uint32_t>) {
        return (seq_t)(((unsigned long long)a * b) % mod);
    } else {
        return (a * b) % mod;   /* arbitrary‑precision or larger integer */
    }
#else
    unsigned __int128 prod = (unsigned __int128)a * b;
    return (seq_t)(prod % mod);
#endif
}

seq_t a(idx_t n)
{
    const seq_t MOD = (seq_t)10000000000ULL;   /* 10^10 */
    const seq_t MUL = (seq_t)3141592221ULL;
    seq_t s = (seq_t)1;                       /* a(1) */

    if (n <= 1) return s;

    for (idx_t i = 2; i <= n; ++i)
    {
        s = mulmod(s, MUL, MOD);   /* compute (3141592221 * s) mod 10^10 */
        s++;                       /* add the +1 term */
        if (s >= MOD) s -= MOD;    /* final modulo 10^10 if needed */
    }
    return s;
}
