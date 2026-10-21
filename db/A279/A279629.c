/*
 * A279629  Coefficients in the expansion of ([s] + [2s]x + [3s]x^2 +
 *          ...)/([r] + [2r]x + [3r]x^2 + ...); [ ] = floor, r = sqrt(3), s =
 *          sqrt(2).
 * https://oeis.org/A279629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 9, i32 n <= 91, i64 n <= 186, i128 n <= 380,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A279629
   a(n) = floor((n+1)*√2) – Σ_{k=1}^{n} a(n-k)·floor(k*√3) ,   a(0)=1
   The formula follows from the coefficient of the quotient of the two
   generating series
         Σ_{k≥1} floor(k√2) x^{k-1}
   over
         Σ_{k≥1} floor(k√3) x^{k-1}.
*/

#include <stdlib.h>

static seq_t *Num = NULL;         /* floor((k+1)*√2)  , k≥0 */
static seq_t *Den = NULL;         /* floor((k+1)*√3)  , k≥0 */
static seq_t *Cache = NULL;       /* a(k) values       */

static idx_t NumCap      = 0;
static idx_t DenCap      = 0;
static idx_t CacheCap    = 0;
static idx_t NumSize     = 0;     /* number of filled entries */
static idx_t DenSize     = 0;
static idx_t CacheSize   = 0;

/* integer square‑root: floor(sqrt(x))   (x ≥ 0) */
static seq_t int_sqrt_seq(seq_t x)
{
    seq_t low = 0, high = x;
    while (low < high) {
        seq_t mid = (low + high + 1) >> 1;
        seq_t sq  = mid * mid;
        if (sq <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* enlarge an array to be able to index at least 'n'. */
static void ensure_capacity(idx_t n, seq_t **arr, idx_t *cap)
{
    if (n < *cap) return;
    idx_t new_cap = (*cap == 0) ? 16 : *cap;
    while (n >= new_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    *arr = tmp;
    *cap = new_cap;
}

/* build Num and Den up to index n (inclusive).  Entries already
   present are left untouched. */
static void ensure_AB(idx_t n)
{
    if (n < NumSize) return;
    ensure_capacity(n, &Num, &NumCap);
    ensure_capacity(n, &Den, &DenCap);

    for (idx_t k = NumSize; k <= n; ++k) {
        seq_t kk = (seq_t)(k + 1);
        seq_t val2 = (seq_t)2 * (kk * kk);   /* 2·(k+1)²  */
        Num[k] = int_sqrt_seq(val2);
        seq_t val3 = (seq_t)3 * (kk * kk);   /* 3·(k+1)²  */
        Den[k] = int_sqrt_seq(val3);
    }
    NumSize = n + 1;
    DenSize = n + 1;
}

/* compute a(i) for all i up to n (inclusive).  Uses a triangular
   convolution with the denominators. */
static void compute_until(idx_t n)
{
    if (n < CacheSize) return;

    ensure_AB(n);

    if (CacheCap == 0) {
        CacheCap = 16;
        Cache = (seq_t *)malloc(CacheCap * sizeof(seq_t));
        if (!Cache) abort();
    }
    while (n >= CacheCap) CacheCap <<= 1;
    Cache = (seq_t *)realloc(Cache, CacheCap * sizeof(seq_t));
    if (!Cache) abort();

    for (idx_t i = CacheSize; i <= n; ++i) {
        if (i == 0) {
            Cache[0] = Num[0];
        } else {
            seq_t sum = 0;
            for (idx_t j = 0; j < i; ++j) {
                sum += Cache[j] * Den[i - j];
            }
            Cache[i] = Num[i] - sum;
        }
    }
    CacheSize = n + 1;
}

/* Public API required by the harness. */
seq_t a(idx_t n)
{
    compute_until(n);
    return Cache[n];
}
