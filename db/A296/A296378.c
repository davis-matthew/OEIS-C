/*
 * A296378  Fibonacci numbers that can be expressed as a sum of two or more
 *          Fibonacci numbers such that no digit appears more than once in the
 *          equation.
 * https://oeis.org/A296378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full, less
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A296378
 * There are very few terms: 3, 5, 8, 13, 34.
 * Here we generate terms by:
 *  * building Fibonacci numbers up to a small limit,
 *  * keeping only those Fibonacci numbers whose decimal digits are distinct,
 *  * testing each new such Fibonacci number to see whether it can be written
 *    as a sum of two or more of the earlier distinct‑digit Fibonacci numbers
 *    with no repeated digits throughout the entire equation.
 * The search is a simple depth‑first subset sum that guarantees at most
 * two occurrences of any digit.  The sequence is finite; once we have
 * examined all Fibonacci numbers up to a fixed bound we stop generating
 * new terms, returning 0 for any index larger than the number of found
 * terms.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static bool distinct_digits(seq_t x)
{
    uint16_t used = 0;
    if (x == 0) return false;      /* not needed, but safe */
    while (x > 0) {
        unsigned d = (unsigned)(x % 10);
        if (used & (1U << d)) return false;
        used |= (1U << d);
        x /= 10;
    }
    return true;
}

static uint16_t digit_mask(seq_t x)
{
    uint16_t mask = 0;
    if (x == 0) return 1U << 0;
    while (x > 0) {
        unsigned d = (unsigned)(x % 10);
        mask |= (1U << d);
        x /= 10;
    }
    return mask;
}

/* Global state for the sequence generator */
static seq_t *fib_list      = NULL;
static uint16_t *mask_list  = NULL;
static size_t fib_cap, fib_len;

static seq_t *results      = NULL;
static size_t res_cap, res_len;

static seq_t fib_prev = 1, fib_curr = 1;
static bool   initialized = false;
static bool   done = false;

/* Forward declaration of DFS helper */
static bool dfs_rep(size_t start, seq_t sum, uint16_t used, int count, seq_t target);

/* Main sequence function */
seq_t a(idx_t n)
{
    /* One‑time initialization */
    if (!initialized) {
        fib_cap = 8;
        fib_list = (seq_t *)malloc(fib_cap * sizeof(seq_t));
        mask_list = (uint16_t *)malloc(fib_cap * sizeof(uint16_t));
        fib_len = 0;

        /* Insert the first Fibonacci number (1) */
        fib_list[fib_len] = 1;
        mask_list[fib_len] = digit_mask(1);
        fib_len++;

        /* Result storage */
        res_cap = 8;
        results = (seq_t *)malloc(res_cap * sizeof(seq_t));
        res_len = 0;

        initialized = true;
    }

    /* Generate terms until we have at least n terms or we are done */
    while (res_len < n && !done) {
        /* Produce next Fibonacci number */
        seq_t next = fib_prev + fib_curr;
        if (next < fib_prev) {            /* overflow */
            done = true;
            break;
        }
        fib_prev = fib_curr;
        fib_curr = next;

        /* Stop after a modest bound – the sequence is known to finish at 34. */
        if (next > (seq_t)1000000) {
            done = true;
            break;
        }

        bool distinct = distinct_digits(next);
        if (distinct) {
            /* Append to the list of candidate Fibonacci numbers */
            if (fib_len == fib_cap) {
                fib_cap <<= 1;
                fib_list = (seq_t *)realloc(fib_list, fib_cap * sizeof(seq_t));
                mask_list = (uint16_t *)realloc(mask_list, fib_cap * sizeof(uint16_t));
            }
            fib_list[fib_len] = next;
            mask_list[fib_len] = digit_mask(next);
            fib_len++;
        }

        /* Test whether 'next' can be represented according to the rule */
        if (!distinct) continue;            /* target itself must have distinct digits */

        uint16_t target_mask = digit_mask(next);
        if (dfs_rep(0, 0, target_mask, 0, next)) {
            /* Append to the results list */
            if (res_len == res_cap) {
                res_cap <<= 1;
                results = (seq_t *)realloc(results, res_cap * sizeof(seq_t));
            }
            results[res_len++] = next;
        }
    }

    if (n == 0 || n > res_len) return 0;   /* sequence is 1‑based */
    return results[n - 1];
}

/* Depth‑first search over subsets of fib_list to reach the target */
static bool dfs_rep(size_t start, seq_t sum, uint16_t used, int count, seq_t target)
{
    if (sum == target) return count >= 2;
    if (sum > target) return false;
    for (size_t i = start; i < fib_len; ++i) {
        seq_t val = fib_list[i];
        if (sum + val > target) break;          /* fib_list is sorted ascending */
        uint16_t m = mask_list[i];
        if (used & m) continue;                /* digit conflict */
        if (dfs_rep(i + 1, sum + val, used | m, count + 1, target))
            return true;
    }
    return false;
}
