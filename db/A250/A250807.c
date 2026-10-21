/*
 * A250807  Number of (n+1) X (3+1) 0..2 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + 6*a(n-3); initial values:
 *            a(1)=225, a(2)=873, a(3)=3081
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 15, u64 n <= 35, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ implementation using std::vector for safe construction
       of arbitrary‑precision internal types. */
    static std::vector<seq_t> memo;
    static idx_t max_computed = 0;

    if (memo.empty()) {
        memo.resize(4);                /* indices 0..3 (0 unused) */
        memo[1] = 225;
        memo[2] = 873;
        memo[3] = 3081;
        max_computed = 3;
    }

    if (n > max_computed) {
        memo.resize(n + 1);
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t val = 6 * memo[i-1]
                       - 11 * memo[i-2]
                       + 6 * memo[i-3];
            memo[i] = val;
        }
        max_computed = n;
    }
    return memo[n];

#else
    /* C implementation with raw realloc/malloc.  The underlying
       seq_t type is assumed to be a trivially constructible POD
       (e.g. uint64_t, __int128, or a simple wrapper).  No
       constructor calls are performed. */
    static seq_t *memo = NULL;
    static idx_t memo_size = 0;
    static idx_t max_computed = 0;

    if (memo_size < 4) {          /* allocate space for indices 0..3 */
        idx_t new_size = 4;
        seq_t *tmp = (seq_t *)realloc(memo, new_size * sizeof(seq_t));
        if (!tmp) return 0;       /* allocation failure */
        memo = tmp;
        memo_size = new_size;
        memo[1] = 225;
        memo[2] = 873;
        memo[3] = 3081;
        max_computed = 3;
    }

    if (n > max_computed) {
        if ((idx_t)(n + 1) > memo_size) {   /* enlarge storage if needed */
            idx_t new_size = n + 1;
            seq_t *tmp = (seq_t *)realloc(memo, new_size * sizeof(seq_t));
            if (!tmp) return 0;             /* allocation failure */
            memo = tmp;
            memo_size = new_size;
            /* new elements are uninitialized; set them to 0 before use */
            for (idx_t i = max_computed + 1; i < new_size; ++i)
                memo[i] = 0;
        }

        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t val = 6 * memo[i-1]
                       - 11 * memo[i-2]
                       + 6 * memo[i-3];
            memo[i] = val;
        }
        max_computed = n;
    }
    return memo[n];
#endif
}
