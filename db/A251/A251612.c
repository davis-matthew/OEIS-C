/*
 * A251612  Number of (n+1)X(2+1) 0..6 arrays with every 2X2 subblock summing
 *          to a prime.
 * https://oeis.org/A251612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of (n+1) × 3 arrays with entries in {0,…,6}
 * such that every 2×2 subblock has a prime sum.
 * The function implements this via a graph walk count on a
 * 343‑node graph whose nodes are all possible 3‑digit rows (base 7).
 * Two rows are adjacent iff all two 2×2 blocks formed with them sum to
 * a prime (primes up to 23).  The number of walks of length n in this
 * graph equals a(n).
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static int            row_digits[343][3];
static bool           primes[25];
static int            adj[343][256];
static size_t         adj_sz[343];
static bool           initialized = false;

static void init(void)
{
    if (initialized) return;
    /* primes up to 23 */
    memset(primes, 0, sizeof(primes));
    int prime_list[] = {2,3,5,7,11,13,17,19,23};
    for (int i = 0; i < (int)(sizeof(prime_list)/sizeof(int)); ++i)
        primes[prime_list[i]] = true;

    /* pre‑compute the 3 digits of each row (base 7) */
    for (int i = 0; i < 343; ++i) {
        int a = i / 49;
        int rem = i % 49;
        int b = rem / 7;
        int c = rem % 7;
        row_digits[i][0] = a;
        row_digits[i][1] = b;
        row_digits[i][2] = c;
    }

    memset(adj_sz, 0, sizeof(adj_sz));

    /* build adjacency lists (symmetric) */
    for (int i = 0; i < 343; ++i) {
        for (int j = i; j < 343; ++j) {
            bool ok = true;
            /* check the two 2×2 blocks */
            for (int k = 0; k < 2; ++k) {
                int sum = row_digits[i][k]   + row_digits[i][k+1] +
                           row_digits[j][k]   + row_digits[j][k+1];
                if (!primes[sum]) { ok = false; break; }
            }
            if (ok) {
                if (adj_sz[i] < 256) adj[i][adj_sz[i]++] = j;
                if (i != j && adj_sz[j] < 256) adj[j][adj_sz[j]++] = i;
            }
        }
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    init();

    seq_t  dp_cur[343];
    seq_t  dp_next[343];

    for (int i = 0; i < 343; ++i) dp_cur[i] = 1;

    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < 343; ++i) dp_next[i] = 0;

        for (int i = 0; i < 343; ++i) {
            seq_t val = dp_cur[i];
            if (val == 0) continue;
            for (size_t k = 0; k < adj_sz[i]; ++k) {
                int j = adj[i][k];
                dp_next[j] += val;
            }
        }
        for (int i = 0; i < 343; ++i) dp_cur[i] = dp_next[i];
    }

    seq_t total = 0;
    for (int i = 0; i < 343; ++i) total += dp_cur[i];
    return total;
}
