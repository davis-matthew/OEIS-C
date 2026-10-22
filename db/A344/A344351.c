/*
 * A344351  Number of squarefree numbers along the main diagonal of an n X n
 *          square array whose elements are the numbers from 1..n^2, listed in
 *          increasing order by rows.
 * https://oeis.org/A344351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} mu(n*k-n+k)^2 (square‑free indicator)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of square‑free integers on the diagonal of the
   n×n array filled with 1…n² in row‑major order.  */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------- prime sieve (once) ---------- */
static bool primes_ready = false;
static uint32_t primes[2000];        /* enough for all n ≤ 10000   */
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (primes_ready) return;

    /* Number of rows needed for n up to 10000. */
    const idx_t LIMIT = 10000;
    bool *is_comp = (bool *)calloc(LIMIT + 1, sizeof(bool));
    for (idx_t i = 2; i * i <= LIMIT; ++i)
        if (!is_comp[i])
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_comp[j] = true;

    size_t cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (!is_comp[i]) primes[cnt++] = (uint32_t)i;

    prime_cnt = cnt;
    free(is_comp);
    primes_ready = true;
}

/* ---------- squarefree test ---------- */
static inline bool is_squarefree(idx_t x)
{
    init_primes();
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint32_t p = primes[i];
        uint64_t sq = (uint64_t)p * p;
        if (sq > x) break;
        if (x % sq == 0) return false;
    }
    return true;
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    seq_t cnt = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t x = (n + 1) * k - n;   /* diagonal value at row k, col k */
        if (is_squarefree(x)) ++cnt;
    }
    return cnt;
}
