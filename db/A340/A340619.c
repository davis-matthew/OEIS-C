/*
 * A340619  n appears A006519(n) times.
 * https://oeis.org/A340619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) is defined recursively by a(1) = 1, and for n > 1 let k be
 *            the unique integer > 0 with k·2^(k‑1) < n ≤ (k+1)·2^k let r = n
 *            - (k+2)·2^(k‑1) then a(n) = 2^k if r = 0 a(n) = 2^k + a(r) if r
 *            > 0 This recurrence yields the OEIS sequence A340619
 * Range:     u8 n <= 1024, u16 n <= 9731, u32 n <= 9731, u64 n <= 9731,
 *            u128 n <= 9731, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)               /* base case for n = 1 */
        return (seq_t)n;

    idx_t cur   = n;          /* current index being processed          */
    seq_t sum   = 0;          /* accumulated value of 2^k terms        */

    while (cur > 1) {
        /* --- find k such that k·2^(k‑1) < cur ≤ (k+1)·2^k --- */
        unsigned int k = 1;
        for (;;) {
            /* use 128‑bit integer to avoid overflow when computing bounds */
            unsigned __int128 bound = ((unsigned __int128)(k + 1)) << k;
            if ((unsigned __int128)cur <= bound)
                break;
            ++k;
        }

        /* add the current 2^k contribution */
        seq_t pow2 = (seq_t)1 << k;
        sum += pow2;

        /* compute base = (k+2)·2^(k‑1)  (also using 128‑bit) */
        unsigned __int128 base128 = ((unsigned __int128)(k + 2)) << (k - 1);

        /* if cur ≤ base, then r = 0 and the recurrence stops */
        if ((unsigned __int128)cur <= base128)
            return sum;

        /* otherwise continue with the remainder r = cur - base */
        cur -= (idx_t)base128;   /* safe cast: base128 ≤ cur ≤ UINT64_MAX */
    }

    /* cur == 1: add the final +1 term */
    return sum + (seq_t)1;
}
