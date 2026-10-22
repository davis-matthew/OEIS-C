/*
 * A326203  Lexicographically earliest sequence such that for all i, j, a(i) =
 *          a(j) => f(i) = f(j), where f(n) = 0 if n > 2 and gcd(n,phi(n)) =
 *          1, with f(n) = n for all other numbers.
 * https://oeis.org/A326203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lexicographically earliest sequence satisfying * a(i) =
 *            a(j) ⇒ f(i) = f(j), f(n)=0 for n>2 with gcd(n,φ(n))=1, * and
 *            f(n)=n for all other numbers
 * Range:     u8 n <= 377, u16 n <= 8585, u32 n <= 8585, u64 n <= 8585,
 *            u128 n <= 8585, big any n
 * Helpers:   gcd, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes required utilities */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/phi.h"
#include "../utils/gcd.h"

static seq_t *cache = NULL;       /* cache[n-1] holds a(n) */
static size_t cache_size = 0;     /* number of elements stored */
static seq_t next_label = 1;      /* next available label */
static seq_t label0 = 0;          /* label used for f(n)=0 */

/* Compute a(n) */
seq_t a(idx_t n)
{
    if (n < 1)                   /* offset is 1; below offset is undefined */
        return 0;

    /* Extend cache if necessary */
    if ((size_t)n > cache_size) {
        size_t new_cap = n * 2;        /* grow roughly twice */
        if (new_cap < (size_t)n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure: return 0 */
        cache = tmp;

        for (size_t k = cache_size; k < (size_t)n; ++k) {
            idx_t idx = (idx_t)(k + 1);  /* actual index */
            bool coprime = false;

            /* f(n) = 0 iff n>2 and gcd(n,φ(n)) = 1 */
            if (idx > 2u) {
                seq_t phi_n = phi(idx);
                if (gcd(idx, phi_n) == 1)
                    coprime = true;
            }

            if (coprime) {
                if (label0 == 0u) {
                    label0 = next_label++;
                }
                cache[k] = label0;
            } else {
                /* f(n) = n (n=1,2 or any non‑coprime n) */
                cache[k] = next_label++;
            }
        }
        cache_size = (size_t)n;
    }
    return cache[n - 1];
}
