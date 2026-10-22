/*
 * A378772  Number of weakly decreasing sequences of positive integers [p1,
 *          p2, ..., p_k] satisfying n = (1 + p1*(1 + p2*(... (1 + p_k)
 *          ...))).
 * https://oeis.org/A378772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5460, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A378772
 *  Number of weakly decreasing sequences of positive integers [p1,p2,…,pk]
 *  satisfying n = (1 + p1*(1 + p2*(…(1 + pk)…)).
 *  a(1) = 1; for n ≥ 2   a(n) = f(n, n-1)
 *  where f(n, m) = Σ_{d| (n-1), d≤m} f((n-1)/d, d)
 *  and f(1, m) = 1 for any m ≥ 1.
 *
 *  The implementation uses memoisation in a simple open-addressing hash
 *  table (static, thread‑safe).  All arithmetic is performed with seq_t,
 *  ensuring exact results for the built‑in integer or arbitrary‑precision
 *  types provided by the build system.
 */

#include <stdbool.h>
#include <stddef.h>

/* Hash table parameters */
#define HASH_TABLE_BITS 19
#define HASH_TABLE_SIZE (1U << HASH_TABLE_BITS)

/* Hash table storage */
static idx_t   ht_n[HASH_TABLE_SIZE];
static idx_t   ht_m[HASH_TABLE_SIZE];
static seq_t   ht_val[HASH_TABLE_SIZE];
static bool    ht_used[HASH_TABLE_SIZE] = {false};

/* Simple multiplicative hash */
static inline size_t
hash_key(idx_t n, idx_t m)
{
    /* 64‑bit fold to table size (power of two) */
    return (size_t)((n * 0x9e3779b97f4a7c15ULL + m) & (HASH_TABLE_SIZE - 1));
}

/* Find or insert value in hash table.
 * If key is absent, *inserted = true and *val = 0 (caller must set value).
 * If key exists, *inserted = false and *val = stored value. */
static bool
hash_get_or_insert(idx_t n, idx_t m, seq_t *val, bool *inserted)
{
    size_t pos = hash_key(n, m);
    for (;;) {
        if (!ht_used[pos]) {
            if (inserted) {
                ht_used[pos] = true;
                ht_n[pos]    = n;
                ht_m[pos]    = m;
                *inserted = true;
                return false;           /* not found, new entry */
            } else {
                return false;           /* not found, caller will insert */
            }
        }
        if (ht_n[pos] == n && ht_m[pos] == m) {
            *inserted = false;
            *val = ht_val[pos];
            return true;                /* found */
        }
        pos = (pos + 1) & (HASH_TABLE_SIZE - 1);
    }
}

/* Recursive dynamic programming with memoisation */
static seq_t
f(idx_t n, idx_t m)
{
    if (n == 1)        /* base case: empty tail */
        return (seq_t)1;

    /* Check memo table */
    seq_t stored;
    bool inserted;
    if (hash_get_or_insert(n, m, &stored, &inserted)) {
        return stored;
    }

    /* Compute value */
    seq_t sum = 0;
    idx_t nm1 = n - 1;

    /* Enumerate all divisors d of nm1 */
    for (idx_t d = 1; d * d <= nm1; ++d) {
        if (nm1 % d == 0) {
            /* First divisor d */
            if (d <= m) {
                seq_t sub = f(nm1 / d, d);
                sum += sub;
            }
            /* Complement divisor nd = nm1 / d */
            idx_t nd = nm1 / d;
            if (nd != d && nd <= m) {
                seq_t sub = f(nm1 / nd, nd);
                sum += sub;
            }
        }
    }

    /* Store in hash table */
    ht_val[hash_key(n, m)] = sum;
    return sum;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    else
        return f(n, n - 1);
}
