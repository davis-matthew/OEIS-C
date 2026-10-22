/*
 * A397993  E.g.f. A(x) satisfies A(-log(1-2*x)/2) = 3*A(x) - 2*x.
 * https://oeis.org/A397993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A397993
   Recurrence:
      a(1) = 1
      a(n+1) = Σ_{k=1..n} 2^(n-k) * |Stirling1(n+1,k)| * a(k)           */
/* The code works for arbitrary precision seq_t (C++ class) and fixed‐size
   unsigned integer types (C).  Static memoisation is used so that calls
   arrive in any order.  The implementation is valid C11 and C++17. */

#include <stdlib.h>
#include <stdint.h>

static idx_t cur_n = 0;   /* highest n computed so far */

/* --------------------------------------------------------------------- */
/* Storage – two versions: one for C, one for C++                       */
/* --------------------------------------------------------------------- */
#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> a_cache;
static std::vector<seq_t> s_arr;
static std::vector<seq_t> pow2_arr;
#else
static seq_t *a_cache   = NULL;
static seq_t *s_arr     = NULL;
static seq_t *pow2_arr  = NULL;
static size_t  a_cap    = 0;
static size_t  s_cap    = 0;
static size_t  pow2_cap = 0;
#endif

/* --------------------------------------------------------------------- */
/* Helper: ensure array has space for indices up to n (inclusive)        */
/* --------------------------------------------------------------------- */
static void ensure_a(idx_t n) {
#ifdef __cplusplus
    if ((size_t)n + 1 <= a_cache.size()) return;
    const size_t new_cap = ((size_t)n + 1) * 2;
    a_cache.resize(new_cap);
#else
    if ((size_t)n + 1 <= a_cap) return;
    size_t new_cap = a_cap ? a_cap : 4;
    while ((size_t)n + 1 > new_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    a_cache = tmp;
    a_cap = new_cap;
    for (size_t i = cur_n + 1; i < new_cap; ++i) a_cache[i] = 0;
#endif
}

static void ensure_s(idx_t n) {
#ifdef __cplusplus
    if ((size_t)n + 1 <= s_arr.size()) return;
    const size_t new_cap = ((size_t)n + 1) * 2;
    s_arr.resize(new_cap);
#else
    if ((size_t)n + 1 <= s_cap) return;
    size_t new_cap = s_cap ? s_cap : 4;
    while ((size_t)n + 1 > new_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(s_arr, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    s_arr = tmp;
    s_cap = new_cap;
    for (size_t i = cur_n + 1; i < new_cap; ++i) s_arr[i] = 0;
#endif
}

static void ensure_pow2(idx_t n) {
    /* need indices up to n, i.e. size > n */
#ifdef __cplusplus
    size_t need = (size_t)n + 1;
    if (need <= pow2_arr.size()) return;
    size_t old_sz = pow2_arr.size();
    pow2_arr.resize(need);
    for (size_t i = old_sz; i < need; ++i)
        pow2_arr[i] = pow2_arr[i-1] * 2;
#else
    if ((size_t)n + 1 <= pow2_cap) return;
    size_t new_cap = pow2_cap ? pow2_cap : 4;
    while ((size_t)n + 1 > new_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(pow2_arr, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    pow2_arr = tmp;
    /* initialise new entries */
    for (size_t i = pow2_cap; i < new_cap; ++i)
        pow2_arr[i] = pow2_arr[i-1] * 2;
    pow2_cap = new_cap;
#endif
}

/* --------------------------------------------------------------------- */
/* Initialise static storage once                                       */
/* --------------------------------------------------------------------- */
static void init_static(void) {
    if (cur_n) return;                     /* already initialised */

#ifdef __cplusplus
    a_cache.resize(2);  a_cache[0] = 0;  a_cache[1] = 1;
    s_arr.resize(2);    s_arr[0] = 0;    s_arr[1] = 1;
    pow2_arr.resize(2); pow2_arr[0] = 1; pow2_arr[1] = 2;
#else
    a_cache   = (seq_t*)malloc(2 * sizeof(seq_t));
    s_arr     = (seq_t*)malloc(2 * sizeof(seq_t));
    pow2_arr  = (seq_t*)malloc(2 * sizeof(seq_t));
    a_cache[0] = 0;  a_cache[1] = 1;
    s_arr[0]   = 0;  s_arr[1]   = 1;
    pow2_arr[0]= 1;  pow2_arr[1]= 2;
    a_cap   = s_cap   = pow2_cap = 2;
#endif
    cur_n = 1;
}

/* --------------------------------------------------------------------- */
/* Main routine                                                        */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* undefined; safe guard */
    init_static();

    if (n <= cur_n) return a_cache[n];

    /* compute from cur_n+1 up to n */
    while (cur_n < n)
    {
        /* ---------- compute unsigned Stirling numbers of 1st kind ---- */
        ensure_s(cur_n + 1);
        // s_arr[1..cur_n] currently hold s(cur_n, k)
        // compute for n+1
        for (idx_t k = cur_n + 1; k >= 1; --k)
        {
            if (k == cur_n + 1) {
                s_arr[k] = 1;                      /* s(n+1, n+1) = 1 */
            } else {
                seq_t add1 = (k == 1) ? 0 : s_arr[k-1];   /* s(n, k-1) */
                seq_t mul  = s_arr[k] * (seq_t)cur_n;     /* n * s(n, k) */
                s_arr[k] = add1 + mul;
            }
        }

        /* ---------- compute a(cur_n+1) -------------------------- */
        ensure_a(cur_n + 1);
        /* powers of 2 needed up to exponent (cur_n-1) */
        ensure_pow2(cur_n - 1);

        seq_t sum = 0;
        for (idx_t k = 1; k <= cur_n; ++k)
        {
            seq_t prod = s_arr[k];
            prod *= a_cache[k];
            prod *= pow2_arr[cur_n - k];
            sum += prod;
        }
        a_cache[cur_n + 1] = sum;

        /* ---- move to next n ----------------------------------- */
        cur_n++;
    }

    return a_cache[n];
}
