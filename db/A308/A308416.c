/*
 * A308416  Values of m for which 2*p + m cannot be a square when p is a
 *          prime.
 * https://oeis.org/A308416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A308416
    a(n) is defined by the following block decomposition
    (OEIS conjecture).  For each integer k > 0 the indices

        S_k = 2(k-1)k + 1   ≤  n  ≤   E_k = 2k(k+1)

    form a block of 4k consecutive values.  Inside a block we count
    t = n – (2k² – 2k)  (1 ≤ t ≤ 4k).  If t ≤ 2k we are in the first
    half of the block; otherwise we are in the second half.  Let

        j = k – (t-1)/2              (in the first half)
        j = k – ((t-2k)-1)/2         (in the second half)
        r = (t-1)  & 1               (in the first half)
        r = ((t-2k)-1) & 1           (in the second half)

    Then

        first half : a = 4k² + 4k – 4j – 3   (r==0)
                     a = 4k² + 4k – 4j       (r==1)

        second half: a = 4k² + 8k – 4j + r

    The function below implements this algorithm using only integer
    arithmetic and an integer square‑root routine for 128‑bit values. */

static inline idx_t isqrt_u128(unsigned __int128 v)
{
    unsigned __int128 lo = 0, hi = ((unsigned __int128)1 << 64); /* hi exclusive */
    while (lo + 1 < hi) {
        unsigned __int128 mid = (lo + hi) >> 1;
        if (mid * mid <= v) {
            lo = mid;
        } else {
            hi = mid;
        }
    }
    return (idx_t)lo;
}

seq_t a(idx_t n)
{
    /* Find k from the inequality 2k²-2k+1 ≤ n ≤ 2k²+2k.
       Use floor(sqrt(2n)) to obtain k = floor((sqrt(2n)+1)/2). */
    unsigned __int128 two_n = (unsigned __int128)n * 2;
    idx_t k = (isqrt_u128(two_n) + 1) >> 1;

    /* base = 2k² – 2k  (fits in 64 bits for all admissible n) */
    unsigned __int128 base128 = (unsigned __int128)2 * k * k
                              - (unsigned __int128)2 * k;
    idx_t base = (idx_t)base128;

    idx_t t = n - base;          /* 1 ≤ t ≤ 4k */

    if (t <= 2 * k) {            /* first half of the block */
        idx_t j = k - (t - 1) / 2;
        int r = (t - 1) & 1;

        seq_t k_sq = (seq_t)k * (seq_t)k;
        seq_t part = 4 * k_sq + 4 * (seq_t)k;

        if (r == 0)
            return part - 4 * (seq_t)j - 3;
        else
            return part - 4 * (seq_t)j;
    } else {                     /* second half of the block */
        idx_t t2 = t - 2 * k;
        idx_t j = k - (t2 - 1) / 2;
        int r = (t2 - 1) & 1;

        seq_t k_sq = (seq_t)k * (seq_t)k;
        seq_t base_val = 4 * k_sq + 8 * (seq_t)k;

        return base_val - 4 * (seq_t)j + r;
    }
}
