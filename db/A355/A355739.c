/*
 * A355739  Number of ways to choose a sequence of all different divisors, one
 *          of each prime index of n (with multiplicity).
 * https://oeis.org/A355739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of injective assignments of divisors of the
 * prime indices of n.  For each prime power p^e‖n, the prime index
 * m = prime_index(p) is repeated e times; each occurrence must be
 * assigned a divisor of m, all assigned divisors are distinct.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- types supplied by the build --------------------------------- */
typedef uint64_t idx_t;     /* index type */

/* --- global tables -------------------------------------------------- */
#define MAX_N 10000
#define MAX_PRIMES 1230          /* number of primes ≤ 10000 */
static uint32_t prime_list[MAX_PRIMES];
static uint16_t prime_index[MAX_N + 1] = {0};   /* 0 if not prime, else 1‑based index */
static size_t prime_count = 0;
static bool primes_initialized = false;

/* divisors of every prime index (1‑based).  maximum divisors ≤ 64. */
static uint16_t divisors[MAX_PRIMES][64];
static uint8_t div_len[MAX_PRIMES] = {0};

static void init_primes(void)
{
    if (primes_initialized) return;

    const int LIMIT = MAX_N;
    bool *sieve = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (!sieve) abort();

    for (int i = 2; i * i <= LIMIT; ++i) {
        if (!sieve[i]) {
            for (int j = i * i; j <= LIMIT; j += i)
                sieve[j] = true;
        }
    }
    for (int i = 2; i <= LIMIT; ++i) {
        if (!sieve[i]) {
            prime_index[i] = (uint16_t)(prime_count + 1);
            prime_list[prime_count++] = (uint32_t)i;
        }
    }
    free(sieve);

    /* precompute divisors of each prime index (1‑based) */
    for (int m = 1; m <= (int)prime_count; ++m) {
        uint8_t len = 0;
        for (int d = 1; d * d <= m; ++d) {
            if (m % d == 0) {
                divisors[m][len++] = (uint16_t)d;
                if (d * d != m)
                    divisors[m][len++] = (uint16_t)(m / d);
            }
        }
        div_len[m] = len;
    }
    primes_initialized = true;
}

/* recursive counting of injective assignments */
static seq_t dfs_recursive(size_t pos, size_t k, const uint32_t *indices,
                           uint16_t *used)
{
    if (pos == k) return (seq_t)1;
    uint32_t m = indices[pos];
    uint8_t len = div_len[m];
    seq_t total = 0;
    for (uint8_t j = 0; j < len; ++j) {
        uint16_t val = divisors[m][j];
        bool conflict = false;
        for (size_t t = 0; t < pos; ++t) {
            if (used[t] == val) {
                conflict = true;
                break;
            }
        }
        if (conflict) continue;
        used[pos] = val;
        total += dfs_recursive(pos + 1, k, indices, used);
    }
    return total;
}

/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_primes();
    if (n < 1) return (seq_t)0;   /* undefined, but safe */
    if (n == 1) return (seq_t)1;

    /* ---- factor n ----------------------------------------------- */
    uint32_t tmp = (uint32_t)n;
    uint32_t primes_f[20];
    int exps[20];
    size_t cnt = 0;
    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * prime_list[i] <= tmp; ++i) {
        uint32_t p = prime_list[i];
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes_f[cnt] = p;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (tmp > 1) {
        primes_f[cnt] = tmp;
        exps[cnt] = 1;
        ++cnt;
    }

    /* ---- build list of prime indices (with multiplicity) --------- */
    uint32_t indices_arr[20];
    size_t pos = 0;
    int freq[MAX_PRIMES] = {0};
    for (size_t i = 0; i < cnt; ++i) {
        uint32_t p = primes_f[i];
        uint16_t idx_m = prime_index[p];   /* 1‑based */
        for (int e = 0; e < exps[i]; ++e) {
            indices_arr[pos++] = idx_m;
            ++freq[idx_m];
        }
    }
    size_t k = pos;

    /* ---- early zero check --------------------------------------- */
    for (size_t i = 0; i < k; ++i) {
        uint32_t m = indices_arr[i];
        if (freq[m] > div_len[m]) return (seq_t)0;
    }

    /* ---- recursion ----------------------------------------------- */
    uint16_t used[20] = {0};
    return dfs_recursive(0, k, indices_arr, used);
}
