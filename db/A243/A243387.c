/*
 * A243387  Number of length n+3 0..6 arrays with no four elements in a row
 *          with pattern abba (with a!=b) and new values 0..6 introduced in
 *          0..6 order.
 * https://oeis.org/A243387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 22*a(n-1) -195*a(n-2) +914*a(n-3) -2539*a(n-4)
 *            +4594*a(n-5) -6074*a(n-6) +6382*a(n-7) -5368*a(n-8) +3726*a(n-9)
 *            -2167*a(n-10) +1026*a(n-11) -419*a(n-12) +130*a(n-13)
 *            -36*a(n-14) +6*a(n-15) -a(n-16) initial values: a(1)=14,
 *            a(2)=46, a(3)=174, a(4)=737, a(5)=3434, a(6)=17366, a(7)=94164,
 *            a(8...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#if !defined(__cplusplus)
#include <stdint.h>  /* for uint64_t, just in case */
#endif

/* Initial 16 terms */
static const seq_t init_vals[16] = {
    (seq_t)14ULL,
    (seq_t)46ULL,
    (seq_t)174ULL,
    (seq_t)737ULL,
    (seq_t)3434ULL,
    (seq_t)17366ULL,
    (seq_t)94164ULL,
    (seq_t)541060ULL,
    (seq_t)3257286ULL,
    (seq_t)20333069ULL,
    (seq_t)130439432ULL,
    (seq_t)853756157ULL,
    (seq_t)5669606647ULL,
    (seq_t)38041540586ULL,
    (seq_t)257118895765ULL,
    (seq_t)1746784622435ULL
};

#if defined(__cplusplus)
/* C++ mode – use a dynamically expanding cache */
static seq_t *cache = nullptr;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

static void ensure_capacity(idx_t need)
{
    if (need <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *new_mem = new seq_t[new_cap];
    for (idx_t i = 0; i < cache_len; ++i) new_mem[i] = cache[i];
    delete[] cache;
    cache = new_mem;
    cache_cap = new_cap;
}
#endif

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */

#if defined(__cplusplus)
    /* C++ path – memoise results */
    if (!cache) {
        /* initial allocation for at least 16 terms */
        idx_t need = n > 16 ? n : 16;
        ensure_capacity(need);
        for (idx_t i = 0; i < 16 && i < n; ++i)
            cache[i] = init_vals[i];
        cache_len = (n < 16) ? n : 16;
    } else if (n > cache_cap) {
        ensure_capacity(n);
    }

    if (n <= cache_len) return cache[n-1];

    /* compute missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 16) {
            val = init_vals[i-1];
        } else {
            val = (seq_t)22 * cache[i-2];
            val -= (seq_t)195 * cache[i-3];
            val += (seq_t)914 * cache[i-4];
            val -= (seq_t)2539 * cache[i-5];
            val += (seq_t)4594 * cache[i-6];
            val -= (seq_t)6074 * cache[i-7];
            val += (seq_t)6382 * cache[i-8];
            val -= (seq_t)5368 * cache[i-9];
            val += (seq_t)3726 * cache[i-10];
            val -= (seq_t)2167 * cache[i-11];
            val += (seq_t)1026 * cache[i-12];
            val -= (seq_t)419 * cache[i-13];
            val += (seq_t)130 * cache[i-14];
            val -= (seq_t)36 * cache[i-15];
            val += (seq_t)6 * cache[i-16];
            val -= cache[i-17];
        }
        cache[i-1] = val;
    }
    cache_len = n;
    return cache[n-1];

#else
    /* Pure C path – compute freshly each call */
    if (n <= 16) return init_vals[n-1];

    seq_t *tmp = (seq_t*)malloc(sizeof(seq_t) * n);
    if (!tmp) abort();   /* allocation failure */

    for (idx_t i = 0; i < 16 && i < n; ++i)
        tmp[i] = init_vals[i];

    for (idx_t i = 17; i <= n; ++i) {
        seq_t val = (seq_t)22 * tmp[i-2];
        val -= (seq_t)195 * tmp[i-3];
        val += (seq_t)914 * tmp[i-4];
        val -= (seq_t)2539 * tmp[i-5];
        val += (seq_t)4594 * tmp[i-6];
        val -= (seq_t)6074 * tmp[i-7];
        val += (seq_t)6382 * tmp[i-8];
        val -= (seq_t)5368 * tmp[i-9];
        val += (seq_t)3726 * tmp[i-10];
        val -= (seq_t)2167 * tmp[i-11];
        val += (seq_t)1026 * tmp[i-12];
        val -= (seq_t)419 * tmp[i-13];
        val += (seq_t)130 * tmp[i-14];
        val -= (seq_t)36 * tmp[i-15];
        val += (seq_t)6 * tmp[i-16];
        val -= tmp[i-17];
        tmp[i-1] = val;
    }

    seq_t res = tmp[n-1];
    free(tmp);
    return res;
#endif
}
