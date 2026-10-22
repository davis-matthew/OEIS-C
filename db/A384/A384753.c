/*
 * A384753  Order of the permutation of {1,...,n} formed by a Josephus
 *          elimination variation: take 2, skip 1.
 * https://oeis.org/A384753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = order of the permutation obtained by the Josephus
 *            elimination “take 2, skip 1”. The permutation is the elements
 *            taken in removal order; a(n) is the least common multiple of the
 *            cycle lengths of this permutation
 * Range:     u8 n <= 31, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* ---------- primes up to 10000 (enough for n ≤ 10000) ---------- */
#define MAX_PRIME 10000
static int primes[MAX_PRIME];
static size_t prime_count = 0;
static bool primes_built = false;

static void build_primes(void)
{
    if (primes_built) return;
    bool sieve[MAX_PRIME + 1];
    memset(sieve, 0, sizeof(sieve));
    for (int i = 2; i * i <= MAX_PRIME; ++i) {
        if (!sieve[i]) {
            for (int j = i * i; j <= MAX_PRIME; j += i)
                sieve[j] = true;
        }
    }
    for (int i = 2; i <= MAX_PRIME; ++i)
        if (!sieve[i])
            primes[prime_count++] = i;
    primes_built = true;
}

/* ----------------------------------------------------------------- */
/*  compute a(n) for n ≥ 1                                           */
/* ----------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety guard, not used by OEIS    */
    build_primes();

    /* -----  build permutation by Josephus “take 2, skip 1”  ----- */
    /* next[i] stores the next element after i in the current circle */
    int *next = (int *)malloc((size_t)(n + 1) * sizeof(int));
    if (!next) return 0;           /* allocation failure              */
    for (idx_t i = 1; i < n; ++i) next[i] = (int)i + 1;
    next[n] = 1;                   /* circular list                   */

    int *perm = (int *)malloc((size_t)n * sizeof(int));
    if (!perm) { free(next); return 0; }

    idx_t current = 1;
    idx_t prev = n;
    size_t pos = 0;
    size_t size = (size_t)n;

    while (size > 0) {
        /* take up to two elements */
        for (int k = 0; k < 2 && size > 0; ++k) {
            perm[pos++] = (int)current;
            int next_elem = next[current];
            next[prev] = next_elem;
            current = next_elem;
            --size;
        }

        /* skip one element if any remain */
        if (size > 0) {
            prev = current;
            current = next[current];
        }
    }

    /* -----  cycle decomposition of the permutation  ----- */
    char *visited = (char *)calloc((size_t)(n + 1), sizeof(char));
    if (!visited) { free(next); free(perm); return 0; }

    int *maxExp = (int *)calloc(prime_count, sizeof(int));

    for (int i = 1; i <= (int)n; ++i) {
        if (visited[i]) continue;
        int len = 0;
        int cur = i;
        do {
            visited[cur] = 1;
            cur = perm[cur - 1];    /* perm is 0‑based, elements are 1‑based */
            ++len;
        } while (cur != i);

        if (len <= 1) continue;    /* fixed point, no effect on LCM */

        /* factorise len and update maximum exponents */
        int rem = len;
        size_t p = 0;
        while (rem > 1 && p < prime_count) {
            int prime = primes[p];
            if (prime * prime > rem) break;
            if (rem % prime == 0) {
                int cnt = 0;
                while (rem % prime == 0) {
                    rem /= prime;
                    ++cnt;
                }
                if (cnt > maxExp[p]) maxExp[p] = cnt;
            }
            ++p;
        }
        if (rem > 1) { /* rem is prime > sqrt(len) */
            for (p = 0; p < prime_count; ++p)
                if (primes[p] == rem) {
                    if (maxExp[p] == 0) maxExp[p] = 1;
                    else if (1 > maxExp[p]) maxExp[p] = 1;
                    break;
                }
        }
    }

    /* -----  compute LCM from prime exponents  ----- */
    seq_t lcm = 1;
    for (size_t p = 0; p < prime_count; ++p) {
        int e = maxExp[p];
        while (e > 0) {
            lcm *= (seq_t)primes[p];
            --e;
        }
    }

    /* free temporary storage */
    free(next);
    free(perm);
    free(visited);
    free(maxExp);

    return lcm;
}
