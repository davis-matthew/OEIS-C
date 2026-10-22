/*
 * A328719  Triangular array read by rows: row n consists of the numbers k
 *          from 1 to n sorted in ascending lexicographic order of their
 *          sequences of p-adic valuations.
 * https://oeis.org/A328719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the a(n) term of OEIS sequence A328719, constructed by
 *            placing the numbers 1..r (where r(r+1)/2 ≥ n) in lexicographic
 *            order of their sequences of p‑adic valuations and reading the
 *            rows sequentially
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Prime generation ---------- */
static idx_t prime_limit = 0;
static uint64_t primes[20000];
static size_t prime_cnt = 0;
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit) return;
    prime_limit = limit;
    prime_cnt = 0;
    for (idx_t p = 2; p <= limit; ++p) {
        bool isprime = true;
        for (idx_t d = 2; d * d <= p; ++d) {
            if (p % d == 0) { isprime = false; break; }
        }
        if (isprime) primes[prime_cnt++] = p;
    }
}

/* ---------- Comparator for lexicographic valuation order ---------- */
static idx_t current_row = 0;
static int compare_lex(const void *a_, const void *b_)
{
    idx_t a = *(const idx_t*)a_;
    idx_t b = *(const idx_t*)b_;
    for (size_t i = 0; i < prime_cnt && primes[i] <= current_row; ++i) {
        idx_t p = primes[i];
        idx_t e1 = 0, t = a;
        while (t % p == 0) { ++e1; t /= p; }
        idx_t e2 = 0; t = b;
        while (t % p == 0) { ++e2; t /= p; }
        if (e1 != e2) return (e1 < e2) ? -1 : 1;
    }
    return 0;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* safety for offset error */

    /* Find row r such that r(r+1)/2 ≥ n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t tri = mid * (mid + 1) / 2;
        if (tri >= n) high = mid;
        else low = mid + 1;
    }
    idx_t r = low;

    /* Position within the row (1‑based) */
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t pos = n - prev_tri;  /* 1‑based position within row r */

    /* Prepare primes up to r */
    ensure_primes(r);
    current_row = r;

    /* Build and sort the row */
    idx_t *row = (idx_t*)malloc(r * sizeof(idx_t));
    for (idx_t i = 0; i < r; ++i) row[i] = i + 1;
    qsort(row, r, sizeof(idx_t), compare_lex);

    seq_t result = (seq_t)row[pos - 1];
    free(row);
    return result;
}
