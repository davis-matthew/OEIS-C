/*
 * A333656  Numbers having at least one 5 in their representation in base 6.
 * https://oeis.org/A333656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th positive integer whose base‑6 representation
 *            contains the digit 5
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Check if a number has a digit 5 in base 6 */
static inline bool has_digit5(seq_t x)
{
    while (x > 0) {
        if ((x % 6) == 5)
            return true;
        x /= 6;
    }
    return false;
}

/* Return the n-th term of sequence A333656 (1‑based index) */
seq_t a(idx_t n)
{
#ifdef __cplusplus
    static std::vector<seq_t> cache;
#else
    static seq_t *cache = NULL;
    static size_t cap = 0;
#endif
    static idx_t count = 0;     /* number of terms stored */
    static seq_t next_val = 5;  /* smallest candidate to test */

    /* Ensure we have at least n terms cached */
    while (count < n) {
        if (has_digit5(next_val)) {
#ifdef __cplusplus
            cache.push_back(next_val);
#else
            if (count == cap) {
                size_t new_cap = (cap == 0) ? 16 : cap * 2;
                seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;
                cache = tmp;
                cap = new_cap;
            }
            cache[count] = next_val;
#endif
            count++;
        }
        next_val++;
    }

#ifdef __cplusplus
    return cache[n - 1];
#else
    return cache[n - 1];
#endif
}
