/*
 * A325820  Multiplication table for carryless product i X j in base 3 for i
 *          >= 0 and j >= 0, read by antidiagonals.
 * https://oeis.org/A325820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) = carryless product in base 3 of the pair (i,j) * where 0 ≤
 *            i,j, i+j = s, and the anti‑diagonal s is chosen so that *
 *            s(s+1)/2 ≤ n < (s+1)(s+2)/2. Indices are read by anti‑diagonals
 * Range:     u8 n <= 712, u16 n <= 8338, u32 n <= 8338, u64 n <= 8338,
 *            u128 n <= 8338, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* carryless product of a and b in base 3 – digits are multiplied
 * modulo 3 and the result is the convolution of the digit sequences. */
static seq_t carryless_mul(seq_t a, seq_t b)
{
    /* extract base‑3 digits of a and b into small arrays */
    seq_t ta = a;
    seq_t tb = b;
    uint32_t da[32];            /* digits of a   */
    uint32_t db[32];            /* digits of b   */
    unsigned len_a = 0;
    unsigned len_b = 0;

    /* a == 0 → len_a stays 0 */
    while (ta != 0) {
        da[len_a++] = (uint32_t)(ta % 3);
        ta /= 3;
    }

    while (tb != 0) {
        db[len_b++] = (uint32_t)(tb % 3);
        tb /= 3;
    }

    /* convolution c_k = Σ a_i * b_j   for i+j = k   (mod 3) */
    unsigned len_c = len_a + len_b;
    uint16_t dc[64] = {0};

    for (unsigned i = 0; i < len_a; ++i) {
        for (unsigned j = 0; j < len_b; ++j) {
            dc[i + j] += (uint16_t)(da[i] * db[j]);   /* ≤ 4, fits in 16 bits */
        }
    }

    /* build the resulting number in base 3 from the digits */
    seq_t result = 0;
    seq_t power  = 1;              /* 3^k */
    for (unsigned k = 0; k < len_c; ++k) {
        uint32_t d = dc[k] % 3;    /* digit modulo 3 */
        result += (seq_t)d * power;
        power *= 3;
    }
    return result;
}

/* locate the anti‑diagonal index s for a given overall index n.
 * Find maximum s with s(s+1)/2 ≤ n.  Uses an integer binary search
 * that works for any n that fits in an idx_t. */
static idx_t antidiagonal_idx(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* OEIS A325820: sequence element at index n. */
seq_t a(idx_t n)
{
    idx_t s      = antidiagonal_idx(n);                    /* anti‑diagonal number */
    idx_t offset = (s * (s + 1)) / 2;                     /* first index of this diagonal */
    idx_t i      = n - offset;                            /* row coordinate   */
    idx_t j      = s - i;                                  /* column coordinate*/
    return carryless_mul((seq_t)i, (seq_t)j);
}
