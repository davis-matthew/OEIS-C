/*
 * A274532  Irregular triangle read by rows: T(n,k) = sum of the elements of
 *          the k-th antidiagonal of the absolute difference table of the
 *          divisors of n.
 * https://oeis.org/A274532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = value at index n of the irregular triangle read by rows
 *            defined as T(n,k)=sum of the k‑th anti‑diagonal of the absolute
 *            difference table of the divisors of n (OEIS A274532)
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#include <vector>
#include <algorithm>
#endif

/* Forward declaration of the sequence function */
seq_t a(idx_t n);

#ifdef __cplusplus

/* ---------- C++ implementation ---------- */

namespace {
    /* Row structure for C++ implementation */
    struct Row {
        idx_t      start;  /* first global index of this row (zero‑based) */
        idx_t      size;   /* number of elements in this row   */
        std::vector<seq_t> values; /* the row values */
    };

    /* Static storage */
    static std::vector<Row>    rows;          /* all computed rows */
    static idx_t               cumCount = 0; /* total number of elements so far */

    /* generate all (sorted) divisors of n, returned as uint64_t vector */
    static std::vector<uint64_t>
    divs_uint64(uint64_t n)
    {
        std::vector<uint64_t> primes;
        std::vector<uint64_t> exps;
        std::vector<uint64_t> divisors{1};

        /* factorisation */
        for (uint64_t p = 2; p * p <= n; ++p) {
            if (n % p == 0) {
                uint64_t exp = 0;
                while (n % p == 0) { n /= p; ++exp; }
                primes.push_back(p);
                exps.push_back(exp);
            }
        }
        if (n > 1) { primes.push_back(n); exps.push_back(1); }

        /* generate all divisors */
        for (size_t i = 0; i < primes.size(); ++i) {
            size_t oldSize = divisors.size();
            uint64_t mult = 1;
            for (uint64_t e = 1; e <= exps[i]; ++e) {
                mult *= primes[i];
                for (size_t j = 0; j < oldSize; ++j)
                    divisors.push_back(divisors[j] * mult);
            }
        }
        std::sort(divisors.begin(), divisors.end());
        return divisors;
    }

    /* compute the full row values for a given n */
    static std::vector<seq_t>
    compute_row(idx_t n)
    {
        std::vector<uint64_t> du = divs_uint64((uint64_t)n);
        size_t m = du.size();
        std::vector<seq_t> antSum(m, (seq_t)0);
        std::vector<seq_t> cur(m);
        std::vector<seq_t> next;
        for (size_t i = 0; i < m; ++i) cur[i] = (seq_t)du[i];

        size_t rowIdx = 0;
        while (true) {
            for (size_t col = 0; col < cur.size(); ++col)
                antSum[rowIdx + col] += cur[col];
            if (cur.size() == 1) break;
            next.clear();
            next.resize(cur.size() - 1);
            for (size_t i = 0; i + 1 < cur.size(); ++i)
                next[i] = cur[i] > cur[i + 1] ? (cur[i] - cur[i + 1])
                                               : (cur[i + 1] - cur[i]);
            cur.swap(next);
            ++rowIdx;
        }
        return antSum;
    }

    /* helper to add a new row to the cache */
    static void add_row(idx_t n)
    {
        std::vector<seq_t> vals = compute_row(n);
        rows.push_back(Row{cumCount, (idx_t)vals.size(), std::move(vals)});
        cumCount += rows.back().size;
    }

    /* locate index within the cached rows */
    static const Row*
    locate_row(idx_t n)
    {
        if (cumCount < n) {
            /* need to compute more rows */
            idx_t nextRow = (idx_t)rows.size() + 1;
            while (cumCount < n) { add_row(nextRow); ++nextRow; }
        }
        idx_t pos = n - 1;
        size_t i = 0;
        while (i < rows.size() && pos >= rows[i].size) {
            pos -= rows[i].size;
            ++i;
        }
        return &rows[i];
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* undefined behaviour for n<1, return 0 */
    const Row* r = locate_row(n);
    return r->values[n - 1 - cumCount + r->size];   /* pos after caching */
}


#else

/* ---------- C implementation ---------- */

#include <string.h>

/* Row structure for C implementation */
typedef struct {
    idx_t      start; /* first global index (zero‑based) */
    idx_t      size;  /* number of elements in this row       */
    seq_t     *values;/* array of row values                   */
} Row;

static size_t rows_count = 0;
static size_t rows_cap   = 0;
static Row    *rows      = NULL;
static idx_t   cumCount  = 0;

/* increase rows array capacity as needed */
static void ensure_rows_cap(size_t needed)
{
    if (rows_cap < needed) {
        size_t newcap = rows_cap ? rows_cap * 2 : 8;
        if (newcap < needed) newcap = needed;
        Row *tmp = realloc(rows, newcap * sizeof(Row));
        if (!tmp) exit(1);
        rows = tmp;
        rows_cap = newcap;
    }
}

/* trial division factorisation, fill primes[] and exp[] */
static void factor_uint64(uint64_t n, uint64_t *primes, uint8_t *exps, size_t *cnt)
{
    size_t idx = 0;
    if (n % 2 == 0) {
        uint8_t e = 0;
        while (n % 2 == 0) { n /= 2; ++e; }
        primes[idx] = 2; exps[idx] = e; ++idx;
    }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            uint8_t e = 0;
            while (n % p == 0) { n /= p; ++e; }
            primes[idx] = p; exps[idx] = e; ++idx;
        }
    }
    if (n > 1) { primes[idx] = n; exps[idx] = 1; ++idx; }
    *cnt = idx;
}

/* compare function for qsort on uint64_t */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t*)a;
    uint64_t vb = *(const uint64_t*)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* generate all divisors and compute row values */
static seq_t*
compute_row(uint64_t n, size_t *outSize)
{
    uint64_t primes[32];
    uint8_t  exps[32];
    size_t   nf = 0;
    factor_uint64(n, primes, exps, &nf);

    /* compute number of divisors */
    size_t m = 1;
    for (size_t i = 0; i < nf; ++i) m *= (exps[i] + 1);

    /* allocate divisor array */
    uint64_t *divs = malloc(m * sizeof(uint64_t));
    if (!divs) exit(1);
    size_t count = 1;
    divs[0] = 1;
    for (size_t i = 0; i < nf; ++i) {
        size_t oldCount = count;
        uint64_t mult = 1;
        for (uint8_t e = 1; e <= exps[i]; ++e) {
            mult *= primes[i];
            for (size_t j = 0; j < oldCount; ++j)
                divs[count++] = divs[j] * mult;
        }
    }
    /* sort divisors */
    qsort(divs, m, sizeof(uint64_t), cmp_uint64);

    /* convert to seq_t values */
    seq_t *vals = malloc(m * sizeof(seq_t));
    if (!vals) exit(1);
    for (size_t i = 0; i < m; ++i) vals[i] = (seq_t)divs[i];
    free(divs);

    /* anti‑diagonal sum array */
    seq_t *ant = calloc(m, sizeof(seq_t));
    if (!ant) exit(1);

    /* current row values */
    seq_t *cur = malloc(m * sizeof(seq_t));
    if (!cur) exit(1);
    memcpy(cur, vals, m * sizeof(seq_t));
    free(vals);  /* vals is no longer needed */

    seq_t *next = NULL;
    size_t len = m, rowIdx = 0;
    while (1) {
        for (size_t col = 0; col < len; ++col)
            ant[rowIdx + col] += cur[col];
        if (len == 1) break;
        size_t nextlen = len - 1;
        if (!next) next = malloc(nextlen * sizeof(seq_t));
        else next = realloc(next, nextlen * sizeof(seq_t));
        for (size_t i = 0; i < nextlen; ++i) {
            seq_t diff = cur[i] > cur[i+1] ? (cur[i] - cur[i+1])
                                          : (cur[i+1] - cur[i]);
            next[i] = diff;
        }
        seq_t *tmp = cur; cur = next; next = tmp; /* swap pointers */
        len = nextlen;
        ++rowIdx;
    }
    free(cur);
    if (next) free(next);
    *outSize = m;
    return ant;
}

/* add a new row to cache */
static void add_row(uint64_t n)
{
    size_t m = 0;
    seq_t *vals = compute_row(n, &m);
    ensure_rows_cap(rows_count + 1);
    rows[rows_count].start  = cumCount;
    rows[rows_count].size   = (idx_t)m;
    rows[rows_count].values = vals;
    rows_count += 1;
    cumCount += m;
}

/* find row containing index n (1‑based) */
static const Row*
locate_row(idx_t n)
{
    if (cumCount < n) {
        uint64_t nextRow = rows_count + 1;
        while (cumCount < n) { add_row(nextRow); ++nextRow; }
    }
    idx_t pos = n - 1;
    size_t i = 0;
    while (i < rows_count && pos >= rows[i].size) {
        pos -= rows[i].size;
        ++i;
    }
    return &rows[i];
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* undefined, return 0 */
    const Row* r = locate_row(n);
    idx_t offset = n - 1 - r->start;
    return r->values[offset];
}

#endif
