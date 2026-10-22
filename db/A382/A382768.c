/*
 * A382768  Number of k < n that are coprime to n and neither squarefree nor
 *          prime powers.
 * https://oeis.org/A382768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = |{ k < n : gcd(k,n)=1 and k is neither squarefree nor a
 *            prime power }|
 * Range:     u8 n <= 718, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static idx_t seen = 0;          /* we have computed membership for [0, seen-1]   */
    static size_t cap = 0;          /* capacity of the membership array             */
    static bool *s = NULL;          /* s[i] == true iff i is in the set S           */

    /* Ensure that s[k] is known for every k < n */
    idx_t need = n > 0 ? n - 1 : 0;
    if (need >= seen) {
        /* Grow the storage if necessary */
        size_t new_cap = (size_t)(need + 1);
        if (new_cap > cap) {
            if (new_cap < cap * 2) new_cap = cap * 2;
            bool *tmp = (bool *)realloc(s, new_cap * sizeof(bool));
            s = tmp;
            cap = new_cap;
        }

        /* Compute membership for the new indices */
        for (idx_t i = seen; i <= need; ++i) {
            if (i < 2) {      /* 0 and 1 are never in S */
                s[i] = false;
                continue;
            }
            uint64_t val = (uint64_t)i;
            uint64_t distinct = 0;
            bool repeat = false;

            for (uint64_t d = 2; d <= val / d; ++d) {
                if (val % d == 0) {
                    ++distinct;
                    uint64_t exp = 0;
                    while (val % d == 0) {
                        val /= d;
                        ++exp;
                    }
                    if (exp >= 2) repeat = true;
                }
            }
            if (val > 1) ++distinct;          /* final prime factor */

            s[i] = repeat && distinct >= 2;
        }
        seen = need + 1;
    }

    /* Count all k < n that are coprime to n and lie in S */
    seq_t ans = 0;
    for (idx_t k = 1; k < n; ++k) {
        if (!s[k]) continue;
        idx_t a1 = k, b1 = n;
        while (b1) {
            idx_t t = a1 % b1;
            a1 = b1;
            b1 = t;
        }
        if (a1 == 1) ++ans;
    }
    return ans;
}
