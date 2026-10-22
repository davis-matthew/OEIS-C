/*
 * A364089  a(n) is the greatest k such that the base-n digits of 2^k are all
 *          distinct.
 * https://oeis.org/A364089
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): the largest k such that the base‑n digits of 2^k are all distinct. */
/* The algorithm repeatedly multiplies by 2 in base n, checks for duplicate\n * digits using a timestamp array, and keeps the largest k with all digits distinct.\n * The search stops when the length of the representation exceeds n, because a\n * longer representation cannot have all distinct digits. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are supplied by the build (e.g. uint64_t, unsigned __int128, ...). */
seq_t a(idx_t n)
{
    /* The offset is 2, but we accept any n >= 2. */
    /* Allocate an array for the base‑n digits of 2^k. */
    size_t cap = 16;
    uint16_t *digits = (uint16_t *)malloc(cap * sizeof(uint16_t));
    if (!digits) return 0;
    digits[0] = 1;          /* 2^0 = 1 */
    size_t len = 1;         /* current number of digits */

    /* Timestamp array to check for duplicate digits quickly. */
    uint32_t *used_ts = (uint32_t *)calloc((size_t)n, sizeof(uint32_t));
    if (!used_ts) {
        free(digits);
        return 0;
    }

    idx_t k = 0;           /* current exponent */
    idx_t best = 0;        /* largest k with all distinct digits */
    uint64_t iter_id = 1;  /* unique id for each check */

    while (len <= n) {
        /* check if current 2^k has distinct digits */
        bool dup = false;
        for (size_t i = 0; i < len; ++i) {
            uint16_t d = digits[i];
            if (used_ts[d] == iter_id) {
                dup = true;
                break;
            }
            used_ts[d] = iter_id;
        }
        iter_id++;
        if (!dup) {
            best = k;
        }

        /* multiply the current number by 2 (in base n) */
        uint16_t carry = 0;
        for (size_t i = 0; i < len; ++i) {
            uint32_t prod = (uint32_t)digits[i] * 2 + carry;
            digits[i] = (uint16_t)(prod % n);
            carry = (uint16_t)(prod / n);
        }
        if (carry) {
            if (len >= cap) {
                cap <<= 1;
                uint16_t *tmp = (uint16_t *)realloc(digits, cap * sizeof(uint16_t));
                if (!tmp) break;          /* allocation failure, abort */
                digits = tmp;
            }
            digits[len++] = carry;
        }
        ++k;
    }

    free(digits);
    free(used_ts);
    return (seq_t)best;
}
