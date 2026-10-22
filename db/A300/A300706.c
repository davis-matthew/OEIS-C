/*
 * A300706  Number of compositions (ordered partitions) of n into squarefree
 *          parts that do not divide n.
 * https://oeis.org/A300706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 58, u64 n <= 108,
 *            u128 n <= 220, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of compositions of n into squarefree parts that do not divide n
 * using dynamic programming.  The DP array is allocated with new/delete in C++ and
 * with malloc/calloc in C, to support arbitrary-precision seq_t types. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Forward declarations for memory helpers */
static seq_t* alloc_seq_array(idx_t count);
static void free_seq_array(seq_t* p, idx_t count);

/* Sieve data for square‑free numbers */
static bool *sqf = NULL;
static idx_t sqf_len = 0;

/* Simple primality test for small integers (used only while building the sieve). */
static bool is_prime_small(int x) {
    if (x < 2) return false;
    for (int d = 2; d * d <= x; ++d)
        if (x % d == 0) return false;
    return true;
}

/* Ensure that sqf[0..limit] indicates square‑free status up to 'limit'. */
static void ensure_sieve(idx_t limit) {
    if (limit < sqf_len) return;               /* already sufficient */
    /* Allocate new array */
    bool *new_sqf = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!new_sqf) exit(EXIT_FAILURE);
    for (idx_t i = 0; i <= limit; ++i) new_sqf[i] = true;
    new_sqf[0] = true;  /* 0 is treated as square‑free for convenience */
    new_sqf[1] = true;

    /* Mark multiples of squares as not square‑free */
    int root = 0;
    while ((idx_t)(root + 1) * (root + 1) <= limit) ++root;
    for (int p = 2; p <= root; ++p) {
        if (!is_prime_small(p)) continue;
        idx_t sq = (idx_t)p * (idx_t)p;
        for (idx_t j = sq; j <= limit; j += sq) new_sqf[j] = false;
    }

    /* Replace old array */
    free(sqf);
    sqf = new_sqf;
    sqf_len = limit + 1;
}

/* Allocate an array of seq_t objects, using new in C++ and calloc in C. */
static seq_t* alloc_seq_array(idx_t count) {
#ifdef __cplusplus
    return new seq_t[count];
#else
    return (seq_t *)calloc((size_t)count, sizeof(seq_t));
#endif
}

/* Free an array of seq_t objects allocated by alloc_seq_array. */
static void free_seq_array(seq_t* p, idx_t count) {
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Determine the allowed parts: squarefree and not dividing n */
    ensure_sieve(n);
    int *parts = (int *)malloc((size_t)(n + 1) * sizeof(int));
    if (!parts) exit(EXIT_FAILURE);
    size_t parts_len = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (!sqf[i]) continue;           /* not square‑free */
        if (n % i == 0) continue;       /* part divides n – disallowed */
        parts[parts_len++] = (int)i;
    }

    /* DP array for compositions */
    seq_t *dp = alloc_seq_array(n + 1);
    if (!dp) exit(EXIT_FAILURE);
    dp[0] = (seq_t)1;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t sum = (seq_t)0;
        for (size_t pi = 0; pi < parts_len; ++pi) {
            int val = parts[pi];
            if ((idx_t)val > k) break;
            sum += dp[(size_t)(k - (idx_t)val)];
        }
        dp[(size_t)k] = sum;
    }

    seq_t result = dp[(size_t)n];

    /* Clean up */
    free(parts);
    free_seq_array(dp, n + 1);
    return result;
}
