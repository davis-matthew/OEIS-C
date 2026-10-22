/*
 * A382418  Numbers with at least one zero in their base-9 representation.
 * https://oeis.org/A382418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base‑9 representation contains at least one 0
 * (OEIS A382418).  The nth number is generated combinatorially:
 *   * the first term (n = 1) is 0;
 *   * for n ≥ 2 we consider the (n‑1)th positive integer whose base‑9
 *     expansion has at least one zero. This integer is reconstructed
 *     from its digit positions by counting the completions of each
 *     prefix.  The algorithm runs in O(L) time, where L is the number
 *     of base‑9 digits (≈ log₉(n)+1). */

/* Include required standard headers. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Return the nth term of OEIS sequence A382418. */
seq_t a(idx_t n)
{
    /* The first term is 0. */
    if (n == 1)
        return (seq_t)0;

    /* For n ≥ 2 we work with the (n-1)th positive number that contains
     * at least one zero in base‑9. n-1 fits into a 64‑bit unsigned integer
     * because it is exactly the index of a positive integer. */
    uint64_t k = (uint64_t)n - 1;          /* 1‑based index of the positive number */

    /* Find the length L (number of base‑9 digits) of that number. */
    size_t L = 1;
    uint64_t pow9 = 1;   /* 9^(L-1) */
    uint64_t pow8 = 1;   /* 8^(L-1) */
    uint64_t cumulative = 0; /* total count of qualifying numbers of lower lengths */

    while (1) {
        /* Count of length‑L numbers that contain at least one 0. */
        uint64_t count = 8ULL * pow9 - 8ULL * pow8; /* 8·9^(L‑1) − 8^L */

        if (cumulative + count >= k)
            break;   /* the desired number has length L */

        cumulative += count;
        pow9 *= 9ULL;
        pow8 *= 8ULL;
        L++;
    }

    /* Remaining count within the length‑L block (1‑based). */
    uint64_t remaining = k - cumulative;

    /* Pre‑compute powers of 9 and 8 up to L for convenient lookup. */
    uint64_t *pow9_arr = (uint64_t *)malloc((L + 1) * sizeof(uint64_t));
    uint64_t *pow8_arr = (uint64_t *)malloc((L + 1) * sizeof(uint64_t));
    if (!pow9_arr || !pow8_arr) {
        /* Memory allocation failed – return 0 as a safe fallback. */
        if (pow9_arr) free(pow9_arr);
        if (pow8_arr) free(pow8_arr);
        return (seq_t)0;
    }

    pow9_arr[0] = 1ULL;
    pow8_arr[0] = 1ULL;
    for (size_t i = 1; i <= L; i++) {
        pow9_arr[i] = pow9_arr[i - 1] * 9ULL;
        pow8_arr[i] = pow8_arr[i - 1] * 8ULL;
    }

    /* Reconstruct the number digit by digit. */
    uint64_t temp_result = 0;  /* intermediate result in 64‑bit to avoid overflow */
    bool zero_seen = false;

    for (size_t pos = 0; pos < L; pos++) {
        int start_digit = (pos == 0) ? 1 : 0;   /* leading digit cannot be 0 */

        for (int d = start_digit; d <= 8; d++) {
            bool next_zero_seen = zero_seen || (d == 0);
            size_t remaining_positions = L - pos - 1;

            uint64_t completions;
            if (next_zero_seen) {
                completions = pow9_arr[remaining_positions];          /* any digits allowed */
            } else {
                completions = pow9_arr[remaining_positions] - pow8_arr[remaining_positions];
            }

            if (remaining <= completions) {
                temp_result = temp_result * 9ULL + (uint64_t)d;
                zero_seen = next_zero_seen;
                break;
            } else {
                remaining -= completions;
            }
        }
    }

    free(pow9_arr);
    free(pow8_arr);

    return (seq_t)temp_result;
}
