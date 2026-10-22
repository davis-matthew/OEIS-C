/*
 * A317526  Smallest "Shift and erase" numbers beginning by (9 + n). See the
 *          Comments section.
 * https://oeis.org/A317526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   a(n) is the smallest integer a that (i) a begins with the
 *            decimal digits (9 + n), and (ii) a/(9+n) equals the number
 *            obtained from a by shifting the leftmost digit to the end and
 *            erasing the digit that was originally second. If no such a
 *            exists for the given n, the function returns 0. The algorit...
 * Range:     u8 none, u16 n <= 1, u32 n <= 1, u64 n <= 1, u128 n <= 1,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* basic integer types */

seq_t a(idx_t n)
{
    /* The sequence only exists for k from 10 to 19. */
    uint64_t k = (uint64_t)n + 9ULL;
    if (k < 10ULL || k > 19ULL)
        return (seq_t)0;   /* no valid number */

    /* Reduce notation: D = 10*k – 1 */
    seq_t D = (seq_t)(10ULL * k - 1ULL);
    seq_t kseq = (seq_t)k;
    seq_t one = (seq_t)1;

    /* Maintain 10^m in full and modulo D */
    seq_t pow10m = (seq_t)10;           /* 10^1 */
    seq_t pow10m_mod = (seq_t)10 % D;   /* 10^1 mod D */
    idx_t m = 1;

    while (1) {
        /* Compute (10^m – 1) mod D */
        seq_t pow10m_minus1_mod = (pow10m_mod + D - one) % D;

        /* Check divisibility: k * (10^m – 1) ≡ 0 (mod D) */
        seq_t k_mod_D = kseq % D;
        seq_t rem = (k_mod_D * pow10m_minus1_mod) % D;
        if (rem == (seq_t)0) {
            /* Exact division holds. Compute tail and a. */
            seq_t tail = (kseq * (pow10m - one)) / D;
            seq_t result = kseq * pow10m + tail;
            return result;
        }

        /* Prepare next m
           10^(m+1) = 10^m * 10
           Modulo D: also multiply by 10. */
        m++;
        pow10m = pow10m * (seq_t)10;
        pow10m_mod = (pow10m_mod * (seq_t)10) % D;

        /* Safety guard: if m becomes excessively large, give up. */
        if (m > 10000ULL)   /* unlikely to happen for k∈[10,19] */
            return (seq_t)0;
    }
}
