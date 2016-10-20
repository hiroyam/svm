namespace classifier {
namespace binary {
/**
 *
 * SVM / SMO algorithm
 *
 * John C. Platt
 * Fast Training of Support Vector Machines using Sequential Minimal Optimization
 *
 */
class svm {
private:
    int                dim;
    int                N;
    float              C;
    float              b;
    float              eps;
    float              tol;
    std::vector<vec_t> data;
    std::vector<int>   label;
    vec_t              a;
    vec_t              E;

public:
    void fit(std::vector<vec_t> &_data, std::vector<int> &_label) {
        data  = _data;
        label = _label;
        dim   = (int)data[0].size();
        N     = (int)data.size();
        C     = 1.0f;
        b     = 0.0f;
        eps   = 0.01f;
        tol   = 0.01f;
        a.resize(N);
        E.resize(N);

        for (int i = 0; i < N; i++) E[i] = -label[i];

        int num_changed = 0;
        int examine_all = 1;

        while (num_changed > 0 || examine_all == 1) {
            num_changed = 0;
            if (examine_all) {
                for (int i = 0; i < N; i++) num_changed += examine_example(i);
            } else {
                for (int i = 0; i < N; i++)
                    if (a[i] != 0 && a[i] != C) num_changed += examine_example(i);
            }

            if (examine_all == 1) {
                examine_all = 0;
            } else if (num_changed == 0) {
                examine_all = 1;
            }
        }
    }

    float predict(vec_t &x) {
        float tmp = 0.0f;
        for (int i = 0; i < N; i++) {
            tmp += a[i] * label[i] * kernel(x, data[i]);
        }
        return tmp + b;
    }

private:
    int examine_example(int i2) {
        int   i1     = 0;
        int   y2     = label[i2];
        float alpha2 = a[i2];
        float E2     = E[i2];
        float r2     = E2 * y2;

        if ((r2 < -tol && alpha2 < C) || (r2 > tol && alpha2 > 0.0f)) {
            int count = 0;
            for (int i = 0; i < N; i++)
                if (a[i] != 0 || a[i] != C) count++;

            if (count > 1) {
                // result of second result heuristic
                vec_t diff(N);
                for (int i = 0; i < N; i++) {
                    diff[i] = abs(E[i] - E2);
                }
                i1 = max_index(diff);

                if (take_step(i1, i2)) return 1;
            }

            // loop over all non-zero and non-C alpha, starting at a random point
            for (int i = uniform_rand(0, N); i < N; i++) {
                if (a[i] != 0 && a[i] != C) i1 = i;
                if (take_step(i1, i2)) return 1;
            }

            // loop over all possible i1, starting at a random point
            for (int i = uniform_rand(0, N); i < N; i++) {
                i1 = i;
                if (take_step(i1, i2)) return 1;
            }
        }
        return 0;
    }

    int take_step(int i1, int i2) {
        if (i1 == i2) return 0;
        float alpha1 = a[i1];
        float alpha2 = a[i2];
        float E1     = E[i1];
        float E2     = E[i2];
        int   y1     = label[i1];
        int   y2     = label[i2];
        int   s      = y1 * y2;

        // compute L, H via equations i3 and i4
        float L, H;
        if (y1 == y2) {
            L = std::max(0.0f, alpha1 + alpha2 - C);
            H = std::min(C, alpha1 + alpha2);
        } else {
            L = std::max(0.0f, alpha2 - alpha1);
            H = std::min(C, C + alpha2 - alpha1);
        }
        if (L == H) return 0;

        float k11 = kernel(data[i1], data[i1]);
        float k12 = kernel(data[i1], data[i2]);
        float k22 = kernel(data[i2], data[i2]);
        float eta = k11 + k22 - 2 * k12;
        float a1  = 0.0f;
        float a2  = 0.0f;

        if (eta > 0) {
            a2 = alpha2 + y2 * (E1 - E2) / eta;
            // printf("%f %f %d %f %f\n", a2, alpha2, y2, E1 - E2, eta);
            if (a2 < L) a2 = L;
            else if (a2 > H) a2 = H;
        } else {
        }

        if (a2 < 1e-8) a2 = 0;
        else if (a2 > C - 1e-8) a2 = C;

        if (std::abs(a2 - alpha2) < eps * (a2 + alpha2 + eps)) return 0;
        a1 = alpha1 + s * (alpha2 - a2);

        // Update threshold to reflect change in Lagrange multipliers
        float b_old = b;
        float b1    = E1 + y1 * (a1 - a[i1]) * k11 + y2 * (a2 - a[i2]) * k12 + b;
        float b2    = E2 + y2 * (a1 - a[i1]) * k12 + y2 * (a2 - a[i2]) * k22 + b;
        if (b1 == b2) b = b1;
        else b = (b1 + b2) / 2;

        // Update weight vector to reflect change in a1 & a2, if SVM is linear
        {}

        // Update error cache using new Lagrange multipliers
        for (int i = 0; i < N; i++) {
            E[i] += y1 * (a1 - a[i1]) * kernel(data[i1], data[i]);
            E[i] += y2 * (a2 - a[i2]) * kernel(data[i2], data[i]);
            E[i] += b_old - b;
        }

        // Store a in the alpha array
        a[i1] = a1;
        a[i2] = a2;
        return 1;
    }

    float kernel(vec_t x1, vec_t x2, float delta = 1.0f) {
        // Radial Basis Function kernel
        float l2 = 0.0f;
        for (int i = 0; i < dim; i++) {
            l2 += std::pow(x1[i] - x2[i], 2);
        }
        return std::exp(-l2 / (delta * delta));
    }
};
}
} // namespace classifier
