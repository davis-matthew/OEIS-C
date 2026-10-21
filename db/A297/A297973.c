/*
 * A297973  Number of nX3 0..1 arrays with every element equal to 2, 3, 4 or 5
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A297973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            4*a(n-1)+10*a(n-2)-22*a(n-3)-58*a(n-4)+51*a(n-5)+136*a(n-6)
 *            -75*a(n-7)-137*a(n-8)+70*a(n-9)+57*a(n-10)-33*a(n-11)-2*a(n-12)+8*a(n-13)
 *            for n > 15, with initial values a(1)=0, a(2)=3, a(3)=8, a(4)=20,
 *            a(5)=124, a(6)=501, a(7)=2410, a(8)=11304, a(9)=53108,
 *            a(10)=251672, a(11)=1189220, a(12)=5...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 30, u128 n <= 59,
 *            big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_seq.h"

static const seq_t _base[16] = {
    0,
    0, 3, 8, 20, 124, 501, 2410, 11304, 53108, 251672,
    1189220, 5629257, 26643680, 126113485, 596993794
};

seq_t a(idx_t n)
{
    if (n <= 15) return _base[n];

    static seq_t *cache = NULL;
    static idx_t cap   = 0;
    static idx_t last  = 0;

    /* initial allocation */
    if (!cache) {
        cap = 64;
        if (cap < n + 1) cap = n + 1;
        cache = alloc_seq(cap);
        for (idx_t i = 1; i <= 15; ++i) cache[i] = _base[i];
        last = 15;
    }

    /* grow array if necessary */
    if (n >= cap) {
        idx_t newcap = cap;
        while (newcap <= n) newcap <<= 1;
        seq_t *tmp = alloc_seq(newcap);
        for (idx_t i = 1; i <= last; ++i) tmp[i] = cache[i];
        /* the old cache can be ignored (leaked) or freed if a free function is available */
        cache = tmp;
        cap = newcap;
    }

    /* compute terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t pos = 0, neg = 0, term;

        /* positive contributions */
        term = cache[i-1] * ((seq_t)4);  pos += term;
        term = cache[i-2] * ((seq_t)10); pos += term;
        term = cache[i-5] * ((seq_t)51); pos += term;
        term = cache[i-6] * ((seq_t)136);pos += term;
        term = cache[i-9] * ((seq_t)70); pos += term;
        term = cache[i-10]* ((seq_t)57);pos += term;
        term = cache[i-13]* ((seq_t)8);  pos += term;

        /* negative contributions */
        term = cache[i-3] * ((seq_t)22); neg += term;
        term = cache[i-4] * ((seq_t)58); neg += term;
        term = cache[i-7] * ((seq_t)75); neg += term;
        term = cache[i-8] * ((seq_t)137);neg += term;
        term = cache[i-11]* ((seq_t)33); neg += term;
        term = cache[i-12]* ((seq_t)2 ); neg += term;

        cache[i] = pos - neg;    /* pos >= neg for all valid n */
    }

    last = n;
    return cache[n];
}
