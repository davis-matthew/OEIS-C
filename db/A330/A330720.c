/*
 * A330720  a(n) is the number of ways of writing the binary expansion of n as
 *          a product (or concatenation) of nonpalindromes.
 * https://oeis.org/A330720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of ways to partition the binary expansion of n into
   non‑palindromic binary substrings.  A DP over all substrings. */
#include <stdint.h>
#include <stdbool.h>

/* a(n) = number of ways of writing the binary expansion of n as a
   concatenation of non‑palindromes.  The helper function is computed
   by dynamic programming over all substrings. */
seq_t a(idx_t n)
{
    /* Determine binary length and bits array (least significant bit last). */
    uint8_t bits[64];        /* bits[0] is most significant bit */
    int L = 0; /* length of binary string */

    if (n == 0) {
        bits[0] = 0;
        L = 1;
    } else {
        /* Find most significant set bit. */
        uint64_t tmp = n;
        int msb = 0;
        while (tmp) {
            tmp >>= 1;
            msb++;
        }
        L = msb;
        /* Extract bits from most to least. */
        for (int i = 0; i < L; ++i) {
            /* bit at position i is bit (L-1-i) from least significant side. */
            bits[i] = (n >> (L - 1 - i)) & 1;
        }
    }

    /* Precompute palindrome table: pal[i][j] is true if bits[i..j-1] is a palindrome. */
    bool pal[64][65] = {false};
    for (int i = 0; i < L; ++i) {
        for (int j = i + 1; j <= L; ++j) {
            bool ok = true;
            for (int k = 0; k < (j - i) / 2; ++k) {
                if (bits[i + k] != bits[j - 1 - k]) {
                    ok = false;
                    break;
                }
            }
            pal[i][j] = ok;
        }
    }

    /* DP: dp[pos] = number of ways to partition suffix starting at pos. */
    seq_t dp[65] = {0};
    dp[L] = 1; /* empty suffix counts as one way for recursion termination */

    for (int i = L - 1; i >= 0; --i) {
        seq_t sum = 0;
        for (int j = i + 1; j <= L; ++j) {
            if (!pal[i][j]) {
                sum += dp[j];
            }
        }
        dp[i] = sum;
    }

    return dp[0];
}
