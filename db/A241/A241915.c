/*
 * A241915  After a(1)=1, numbers 1 .. A061395(n), followed by numbers 1 ..
 *          A061395(n+1), etc.
 * https://oeis.org/A241915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9254, u16 n <= 9254, u32 n <= 9254, u64 n <= 9254,
 *            u128 n <= 9254, big any n
 * Helpers:   build_primes, largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A241915:  for i = 1,2,3,...
   let L(i) = index of the largest prime factor of i
   (for i = 1 we use L(1) = 1 by convention).
   Append the block 1,2,…,L(i) to the sequence.
   a(n) is the 1‑based offset of n within the block that
   contains position n. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes.h"
#include "../utils/largest_prime_factor.h"

static idx_t *prime_list   = NULL;
static size_t prime_count  = 0;

static idx_t *block_sizes = NULL;
static idx_t *cumulative  = NULL;
static size_t block_alloc = 0;
static size_t block_used  = 0;

/* Build a reasonable list of primes once. 2 000 000 is ample for n ≤ 10 000. */
static void init_primes(void)
{
    if (prime_list) return;
    build_primes((idx_t)2000000, &prime_list, &prime_count);
}

/* Binary search for the 1‑based index of a prime in prime_list. */
static idx_t prime_index(idx_t p)
{
    size_t lo = 0, hi = prime_count - 1;
    while (lo <= hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] == p) return (idx_t)(mid + 1);
        if (prime_list[mid] < p) lo = mid + 1;
        else                    hi = mid - 1;
    }
    return (idx_t)0;  /* should never happen for a prime */
}

/* Ensure storage for at least 'need' blocks. */
static void ensure_capacity(size_t need)
{
    if (need <= block_alloc) return;
    size_t new_cap = block_alloc ? block_alloc << 1 : 16;
    if (new_cap < need) new_cap = need;
    block_sizes = (idx_t *)realloc(block_sizes, new_cap * sizeof(idx_t));
    cumulative  = (idx_t *)realloc(cumulative,  new_cap * sizeof(idx_t));
    block_alloc = new_cap;
}

/* Generate blocks until the cumulative length is ≥ n. */
static void extend_blocks_until(idx_t n)
{
    init_primes();
    for (idx_t i = (idx_t)block_used + 1; ; ++i) {
        idx_t lpf = (i == 1) ? 1 : (idx_t)largest_prime_factor((uint64_t)i);
        idx_t len = (lpf <= 1) ? (idx_t)1 : prime_index(lpf);
        ensure_capacity(block_used + 1);
        block_sizes[block_used] = len;
        if (block_used == 0) cumulative[block_used] = len;
        else                 cumulative[block_used] = cumulative[block_used - 1] + len;
        ++block_used;
        if (cumulative[block_used - 1] >= n) break;
    }
}

/* Find the block (0‑based index) that contains position n. */
static size_t block_of_position(idx_t n)
{
    size_t lo = 0, hi = block_used - 1;
    while (lo <= hi) {
        size_t mid = (lo + hi) >> 1;
        if (cumulative[mid] >= n) {
            if (mid == 0) return 0;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return lo;  /* should not exceed block_used - 1 */
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    if (block_used == 0 || cumulative[block_used - 1] < n)
        extend_blocks_until(n);
    size_t blk = block_of_position(n);
    idx_t prev = (blk == 0) ? (idx_t)0 : cumulative[blk - 1];
    return (seq_t)(n - prev);
}
