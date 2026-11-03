/*
 * A384778  Consecutive states of the linear congruential pseudo-random number
 *          generator 55151000561141*s mod 2^48 when started at 1.
 * https://oeis.org/A384778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 55151000561141^(n-1) mod 2^48, a(1)=1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 32 terms through n = 9712 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper to multiply two seq_t values modulo 2^48.
   It uses 128‑bit arithmetic when available to avoid overflow. */
static inline seq_t modmul48(seq_t a, seq_t b)
{
    /* mask 48 lower bits: 0xFFFFFFFFFFFFULL */
    const seq_t mask48 = (seq_t)(((uint64_t)1 << 48) - 1);

#ifdef __SIZEOF_INT128__
    /* 128‑bit product then mask */
    return ((unsigned __int128)a * b) & mask48;
#else
    /* Best effort if 128‑bit types are unavailable */
    return (a * b) & mask48;
#endif
}

/* Linear congruential generator recurrence:
   a(1) = 1
   a(n) = 55151000561141 * a(n-1) mod 2^48  for n>1
   Equivalent to a(n) = 55151000561141^(n-1) mod 2^48. */
seq_t a(idx_t n)
{
    if (n == 0)     /* undefined, but return 0 for safety */
        return (seq_t)0;
    if (n == 1)     /* first term explicitly given */
        return (seq_t)1;

    /* Base multiplier of the LCG, already less than 2^48 */
    const seq_t base0 = (seq_t)55151000561141ULL;

    seq_t result = (seq_t)1;          /* value of m^(0) */
    seq_t base   = base0;             /* value of m^(1) */
    idx_t e = n - 1;                  /* exponent */

    while (e) {
        if (e & 1)
            result = modmul48(result, base);
        e >>= 1;
        if (e)
            base = modmul48(base, base);
    }
    return result;
}
