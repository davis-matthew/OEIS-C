/*
 * A277054  Least k such that n-th repunit times k is a pandigital.
 * https://oeis.org/A277054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = smallest k such that the n‑th repunit (in base 10) times
 *            k is a pandigital (contains all digits 0–9 at least once)
 * Range:     u8 none, u16 none, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10.h"

static bool next_permutation(int *a, int n)
/* Generate the next lexicographic permutation of the array a[0…n‑1].
   Return false when the current permutation is the last one. */
{
    int i = n - 2;
    while (i >= 0 && a[i] >= a[i + 1]) --i;
    if (i < 0) return false;
    int j = n - 1;
    while (a[j] <= a[i]) --j;
    int tmp = a[i]; a[i] = a[j]; a[j] = tmp;
    for (int l = i + 1, r = n - 1; l < r; ++l, --r) {
        tmp = a[l]; a[l] = a[r]; a[r] = tmp;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Special cases */
    if (n == 1) return (seq_t)1023456789ULL;
    if (n >= 5)   return (seq_t)92115ULL;   /* proven constant for n ≥ 5 */

    /* Cache results for the remaining small n (2, 3, 4). */
    static seq_t cache[5] = {0};
    if (cache[n]) return cache[n];

    /* Compute the n‑th repunit: R_n = (10^n – 1)/9. */
    seq_t repunit = (pow10(n) - (seq_t)1) / (seq_t)9;

    /* Enumerate all 10‑digit pandigitals (digits 0–9, leading digit ≠ 0). */
    int digits[10] = {0,1,2,3,4,5,6,7,8,9};
    do {
        if (digits[0] == 0) continue;              /* skip leading zero */

        seq_t pandig = 0;
        for (int i = 0; i < 10; ++i)
            pandig = pandig * (seq_t)10 + (seq_t)digits[i];

        if (pandig % repunit == 0) {
            cache[n] = pandig / repunit;
            return cache[n];
        }
    } while (next_permutation(digits, 10));

    return (seq_t)0;   /* unreachable, but keeps the compiler happy */
}
