/*
 * A250697  Number of (6+1)X(n+1) 0..3 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 30*a(n-1) -415*a(n-2) +3514*a(n-3) -20386*a(n-4)
 *            +85924*a(n-5) -272198*a(n-6) +661180*a(n-7) -1244717*a(n-8)
 *            +1822478*a(n-9) -2068955*a(n-10) +1802474*a(n-11)
 *            -1181760*a(n-12) +563888*a(n-13) -184752*a(n-14) +37152*a(n-15)
 *            -3456*a(n-16) Initial values: a(1) = 233300, a(2) = 1647460,
 *            a(3) =...
 * Range:     u8 none, u16 none, u32 n <= 7, u64 n <= 25, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    static const seq_t init_vals[16] = {
        (seq_t)233300ULL,
        (seq_t)1647460ULL,
        (seq_t)8474002ULL,
        (seq_t)36673634ULL,
        (seq_t)143136866ULL,
        (seq_t)523260542ULL,
        (seq_t)1832718604ULL,
        (seq_t)6241479780ULL,
        (seq_t)20880237460ULL,
        (seq_t)69123618848ULL,
        (seq_t)227638279630ULL,
        (seq_t)748479312062ULL,
        (seq_t)2463064051982ULL,
        (seq_t)8124116088970ULL,
        (seq_t)26881398786600ULL,
        (seq_t)89272256311120ULL
    };

    if (n <= 16) {
        return init_vals[n - 1];
    }

    seq_t last[16];
    for (int i = 0; i < 16; ++i) {
        last[i] = init_vals[i];
    }

    for (idx_t k = 17; k <= n; ++k) {
        seq_t res = (seq_t)30 * last[15];
        res -= (seq_t)415 * last[14];
        res += (seq_t)3514 * last[13];
        res -= (seq_t)20386 * last[12];
        res += (seq_t)85924 * last[11];
        res -= (seq_t)272198 * last[10];
        res += (seq_t)661180 * last[9];
        res -= (seq_t)1244717 * last[8];
        res += (seq_t)1822478 * last[7];
        res -= (seq_t)2068955 * last[6];
        res += (seq_t)1802474 * last[5];
        res -= (seq_t)1181760 * last[4];
        res += (seq_t)563888 * last[3];
        res -= (seq_t)184752 * last[2];
        res += (seq_t)37152 * last[1];
        res -= (seq_t)3456 * last[0];

        for (int i = 0; i < 15; ++i) {
            last[i] = last[i + 1];
        }
        last[15] = res;
    }

    return last[15];
}
