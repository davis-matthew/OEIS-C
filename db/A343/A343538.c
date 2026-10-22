/*
 * A343538  Longest sequence of distinct two-digit numbers (10-99) such that
 *          any two or three consecutive terms can be concatenated into a
 *          prime number, even when connected into a loop. Smallest number is
 *          listed first.
 * https://oeis.org/A343538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   cycle_is_smaller, rotate_min
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   This implementation computes the longest cycle of distinct two‑digit
   numbers (10–99) such that every pair of consecutive terms and every
   triple of consecutive terms, even when the cycle is wrapped around,
   concatenates to a prime number.  The sequence is the lexicographically
   smallest such cycle after rotating it so that its minimal element
   comes first.  The function `a` returns the n‑th member of that cycle
   (1‑based).  If n is larger than the cycle length the function returns 0.
*/

#include <stdint.h>
#include <stdbool.h>
#include "../utils/cycle_is_smaller.h"
#include "../utils/rotate_min.h"

#define NUM_NODES 90                /* two‑digit numbers 10 … 99 */
#define MAX_PRIME 1000000           /* 6‑digit primes fit in this range */

/* ---------------------------------------------------------------------- */
/*  Prime sieve up to MAX_PRIME                                            */
/* ---------------------------------------------------------------------- */
static bool prime_sieve[MAX_PRIME + 1];
static bool primes_ready = false;

static void init_prime_sieve(void)
{
    if (primes_ready) return;
    primes_ready = true;
    for (int i = 0; i <= MAX_PRIME; ++i) prime_sieve[i] = true;
    prime_sieve[0] = prime_sieve[1] = false;
    for (int i = 2; i * i <= MAX_PRIME; ++i)
        if (prime_sieve[i])
            for (int j = i * i; j <= MAX_PRIME; j += i)
                prime_sieve[j] = false;
}

/* ---------------------------------------------------------------------- */
/*  Edge / triple tests                                                     */
/* ---------------------------------------------------------------------- */
static inline bool pair_prime(int a, int b)   /* a,b are two‑digit numbers 10–99 */
{
    return prime_sieve[a * 100 + b];
}

static inline bool triple_prime(int a, int b, int c)
{
    return prime_sieve[a * 10000 + b * 100 + c];
}

/* ---------------------------------------------------------------------- */
/*  Graph construction: adjacency lists for pair primes                   */
/* ---------------------------------------------------------------------- */
static int adj[NUM_NODES][NUM_NODES];
static int adj_cnt[NUM_NODES];

static void build_graph(void)
{
    init_prime_sieve();
    for (int i = 0; i < NUM_NODES; ++i) {
        int a = 10 + i;
        adj_cnt[i] = 0;
        for (int j = 0; j < NUM_NODES; ++j) {
            int b = 10 + j;
            if (pair_prime(a, b))
                adj[i][adj_cnt[i]++] = j;
        }
    }
}

/* ---------------------------------------------------------------------- */
/*  Utilities for cycle handling                                            */
/* ---------------------------------------------------------------------- */
static bool visited[NUM_NODES];
static int path[NUM_NODES];
static int path_len;

static int best_path[NUM_NODES];
static int best_len = 0;

/* ---------------------------------------------------------------------- */
/*  Depth‑first search for longest cycle                                  */
/* ---------------------------------------------------------------------- */
static void dfs(int start, int cur)
{
    /* pruning: even if we add all remaining nodes we cannot beat best */
    int remaining = NUM_NODES - path_len;
    if (path_len + remaining <= best_len) return;

    for (int k = 0; k < adj_cnt[cur]; ++k) {
        int nxt = adj[cur][k];

        if (nxt == start) { /* attempt to close the cycle */
            if (path_len >= 2) {
                int last1 = path[path_len - 2];
                int last2 = path[path_len - 1];
                int a = 10 + last1;
                int b = 10 + last2;
                int c = 10 + start;
                if (!triple_prime(a, b, c)) continue;
                int d = 10 + start;
                int e = 10 + path[1];
                int f = 10 + path[2];
                /* For cycles of length 3, path[2] is path[1] (no, we need index 1) */
                int g = 10 + path[1];
                /* Need to check triples that wrap around:
                   (last1, last2, start) and (last2, start, first_after_start)
                   where first_after_start = path[1] (if exists) */
                int x_next = 10 + path[1];
                if (!triple_prime(a, b, c)) continue;          /* last1,last2,start */
                if (!triple_prime(b, c, x_next)) continue;     /* last2,start,first+1 */

                int len = path_len;
                int tmp[NUM_NODES];
                rotate_min(path, tmp, len);
                if (len > best_len) {
                    best_len = len;
                    for (int i = 0; i < len; ++i) best_path[i] = tmp[i];
                } else if (len == best_len) {
                    if (cycle_is_smaller(tmp, best_path, len)) {
                        for (int i = 0; i < len; ++i) best_path[i] = tmp[i];
                    }
                }
            }
            continue;
        }

        if (visited[nxt]) continue;

        if (path_len >= 2) {
            int a = 10 + path[path_len - 2];
            int b = 10 + path[path_len - 1];
            int c = 10 + nxt;
            if (!triple_prime(a, b, c)) continue;
        }
        visited[nxt] = true;
        path[path_len++] = nxt;
        dfs(start, nxt);
        path_len--;
        visited[nxt] = false;
    }
}

static void compute_best_cycle(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    build_graph();

    for (int start = 0; start < NUM_NODES; ++start) {
        for (int i = 0; i < NUM_NODES; ++i) visited[i] = false;
        path_len = 0;
        path[path_len++] = start;
        visited[start] = true;
        dfs(start, start);
    }
}

/* ---------------------------------------------------------------------- */
/*  Public function according to the interface                          */
/* ---------------------------------------------------------------------- */
/*  seq_t receives the output; cast from internal int values. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    compute_best_cycle();
    if (n > (idx_t)best_len) return 0;
    return (seq_t)(10 + best_path[(int)n - 1]);
}
