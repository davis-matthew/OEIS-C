/*
 * A249855  Numbers <= 10^6 with valid Luhn mod 10 check digit, sorted first
 *          by check digit, then lexicographically.
 * https://oeis.org/A249855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, look
 * Formula:   a(n) = the nth element (1‑based) of the list of all integers
 *            ≤10⁶ whose decimal representation passes the Luhn mod 10 check,
 *            sorted first by the last decimal digit (the Luhn check‑digit),
 *            then lexicographically as decimal strings
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 9428, u64 n <= 9428,
 *            u128 n <= 9428, big any n
 * Helpers:   luhn_valid_uint, uint32_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation simply pre‑computes the list on first call
   and looks it up for subsequent calls. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/luhn_valid_uint.h"
#include "../utils/uint32_to_str.h"

/* Comparator for the pre‑computed integers:
   first by last digit (check‑digit), then lexicographically. */
static int cmp_uint32(const void *p1, const void *p2)
{
    const uint32_t *a = (const uint32_t *)p1;
    const uint32_t *b = (const uint32_t *)p2;

    uint32_t da = *a % 10;
    uint32_t db = *b % 10;
    if (da != db)
        return (da < db) ? -1 : 1;

    /* Lexicographic comparison of decimal strings */
    char buf_a[8], buf_b[8];
    int len_a, len_b;
    uint32_to_str(*a, buf_a, &len_a);
    uint32_to_str(*b, buf_b, &len_b);

    int cmp = memcmp(buf_a, buf_b, (len_a < len_b) ? len_a : len_b);
    if (cmp != 0)
        return (cmp < 0) ? -1 : 1;
    return (len_a < len_b) ? -1 : (len_a > len_b ? 1 : 0);
}

seq_t a(idx_t n)
{
    /* State for one‑time precomputation */
    static bool initialized = false;
    static uint32_t *nums = NULL;
    static size_t count = 0;

    if (!initialized) {
        const uint32_t LIMIT = 1000000u;
        size_t cap = 128;
        nums = (uint32_t *)malloc(cap * sizeof(uint32_t));
        count = 0;

        for (uint32_t i = 0; i <= LIMIT; ++i) {
            if (luhn_valid_uint(i)) {
                if (count == cap) {
                    cap <<= 1;
                    nums = (uint32_t *)realloc(nums, cap * sizeof(uint32_t));
                }
                nums[count++] = i;
            }
        }

        /* Sort the list by check digit and lexicographic order. */
        qsort(nums, count, sizeof(uint32_t), cmp_uint32);

        initialized = true;
    }

    if (n == 0 || (size_t)n > count)
        return 0; /* undefined / out‑of‑range */

    /* Cast to seq_t for the caller; overflow in small seq_t types is
       acceptable because those builds only test values that fit. */
    return (seq_t)nums[n - 1];
}
