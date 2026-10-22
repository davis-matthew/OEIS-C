/*
 * A385701  Numbers such that when the leftmost digit is moved to the unit's
 *          place the result is divisible by 6.
 * https://oeis.org/A385701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 9993, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers such that moving the leftmost decimal digit to the units place
   results in a number divisible by 6.  This is equivalent to picking
   numbers whose leading digit is even and whose digit sum is divisible by 3. */

#include <stdlib.h>
#include <stdbool.h>

/* Helper: true if m satisfies the condition */
static inline bool is_special(seq_t m)
{
    if (m == 0)
        return true;                            /* 0 -> 0, divisible by 6 */

    seq_t temp = m;
    int sum_mod3 = 0;
    int first = 0;

    while (temp >= 10) {
        seq_t d = temp % 10;
        sum_mod3 = (sum_mod3 + (int)(d % 3)) % 3;
        temp /= 10;
    }
    first = (int)(temp % 10);                    /* leading digit */
    sum_mod3 = (sum_mod3 + first) % 3;

    return ((first & 1) == 0) && (sum_mod3 == 0); /* even leading digit & sum % 3 == 0 */
}

/* Cache for already found terms */
static seq_t *cache = NULL;
static idx_t  count = 0;      /* number of terms stored */
static size_t cap   = 0;      /* current allocated size */

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Deliver from cache if already computed */
    if (n <= count)
        return cache[n - 1];

    /* Find new terms until we have at least n terms */
    seq_t candidate = (count ? cache[count - 1] + 1 : 0);

    /* Ensure cache capacity grows as needed */
    if (cap == 0) {
        cap = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)
            abort(); /* allocation failure */
    }

    while (count < n) {
        if (count == cap) {
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                abort(); /* allocation failure */
            cache = tmp;
            cap = new_cap;
        }

        if (is_special(candidate)) {
            cache[count++] = candidate;
        }
        ++candidate; /* move to next integer */
    }

    return cache[n - 1];
}
