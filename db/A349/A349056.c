/*
 * A349056  Number of weakly alternating permutations of the multiset of prime
 *          factors of n.
 * https://oeis.org/A349056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of weakly alternating permutations of the multiset
 *            of prime factors of n. Computed by recursive backtracking that
 *            honors the alternating inequality pattern for both starting
 *            directions
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Recursive search for weakly alternating permutations.
   pr      – array of distinct prime values (ascending).
   cnt     – current remaining multiplicities of each prime.
   d       – number of distinct primes.
   rem     – number of elements still to place.
   last    – index (in pr) of the element just placed.
   dir     – expected direction for the next comparison:
             0 = previous element <= next element,
             1 = previous element >= next element.
   Returns the number of valid completions from this state. */
static seq_t dfs(const uint64_t *pr, int *cnt, int d,
                 int rem, int last, int dir)
{
    if (rem == 0)
        return (seq_t)1;

    seq_t total = 0;
    uint64_t last_val = pr[last];

    for (int i = 0; i < d; ++i) {
        if (cnt[i] == 0)
            continue;
        uint64_t val = pr[i];

        /* Check that the comparison between last_val and val
           satisfies the expected direction.  Equality satisfies
           both directions, so no special handling is needed. */
        if ((dir == 0 && !(last_val <= val)) ||
            (dir == 1 && !(last_val >= val)))
            continue;

        cnt[i]--;
        total += dfs(pr, cnt, d, rem - 1, i, dir ^ 1);
        cnt[i]++;
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;        /* a(0) is unused; a(1)=1 */

    /* Factorisation of n.  The helper may supply up to 12 primes. */
    uint64_t primes[12];
    uint64_t exps[12];
    size_t num = 0;
    factor_u64(n, primes, exps, &num);

    if (num == 0)           /* n == 1 */
        return (seq_t)1;

    int d = (int)num;
    int cnt[12];
    int total_factors = 0;
    for (int i = 0; i < d; ++i) {
        cnt[i] = (int)exps[i];
        total_factors += cnt[i];
    }

    /* All factors equal (n is a prime power).  Only one distinct
       permutation exists, and it is weakly alternating. */
    if (d == 1)
        return (seq_t)1;

    seq_t total = 0;
    /* Count permutations for both possible starting directions
       (start with <= or start with >=). */
    for (int start_dir = 0; start_dir <= 1; ++start_dir) {
        for (int i = 0; i < d; ++i) {
            if (cnt[i] == 0)
                continue;
            cnt[i]--;
            total += dfs(primes, cnt, d,
                         total_factors - 1, i, start_dir);
            cnt[i]++;
        }
    }
    return total;
}
