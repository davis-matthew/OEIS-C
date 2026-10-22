/*
 * A303883  Number of nX3 0..1 arrays with every element unequal to 0, 2, 3 or
 *          4 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A303883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) satisfies the linear recurrence * * a(n) = 3*a(n-1) +
 *            3*a(n-2) - 10*a(n-3) + 4*a(n-5) * - 16*a(n-6) + 5*a(n-7) +
 *            17*a(n-8) - 5*a(n-9) * + 3*a(n-10) + 7*a(n-11) + 7*a(n-12) +
 *            9*a(n-13) * + 3*a(n-14) + 5*a(n-15) + 2*a(n-16) , for n > 18 * *
 *            Initial values: * a(1)=1, a(2)=8, a(3)=11, a(4)=36,...
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_values[18] = {
    1ULL,   8ULL,   11ULL,   36ULL,   87ULL,
   256ULL,  684ULL, 1932ULL, 5308ULL, 14809ULL,
 41025ULL,114088ULL,316733ULL,880136ULL,2444724ULL,
6792145ULL,18868672ULL,52420409ULL
};

static seq_t *rec_cache = NULL;
static idx_t rec_cache_upto = 0;

/* Helper to ensure the cache has at least 'need' entries (1‑based). */
static void ensure_rec_cache(idx_t need)
{
    if (rec_cache_upto >= need) return;

    size_t new_size = (size_t) (need + 1);
    if (!rec_cache) {
        rec_cache = (seq_t*)malloc(new_size * sizeof(seq_t));
    } else if (new_size > rec_cache_upto + 1) {
        rec_cache = (seq_t*)realloc(rec_cache, new_size * sizeof(seq_t));
    }

    /* Fill missing entries from the initial values. */
    for (idx_t i = rec_cache_upto + 1; i <= need; ++i) {
        if (i <= 18) {
            rec_cache[i] = init_values[i - 1];
        } else {
            /* Compute using the recurrence. */
            seq_t pos = 0, neg = 0;

            pos += 3 * rec_cache[i - 1];
            pos += 3 * rec_cache[i - 2];
            pos += 4 * rec_cache[i - 5];
            pos += 5 * rec_cache[i - 7];
            pos += 17 * rec_cache[i - 8];
            pos += 3 * rec_cache[i -10];
            pos += 7 * rec_cache[i -11];
            pos += 7 * rec_cache[i -12];
            pos += 9 * rec_cache[i -13];
            pos += 3 * rec_cache[i -14];
            pos += 5 * rec_cache[i -15];
            pos += 2 * rec_cache[i -16];

            neg += 10 * rec_cache[i - 3];
            neg += 16 * rec_cache[i - 6];
            neg += 5  * rec_cache[i - 9];

            rec_cache[i] = pos - neg;
        }
    }

    rec_cache_upto = need;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out of offset, behaviour undefined; return 0 */

    ensure_rec_cache(n);
    return rec_cache[n];
}
