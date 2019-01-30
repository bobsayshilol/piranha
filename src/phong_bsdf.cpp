#include <phong_bsdf.h>

manta::PhongBSDF::PhongBSDF() {
	m_power = 1.0;
}

manta::PhongBSDF::~PhongBSDF() {

}

manta::math::Vector manta::PhongBSDF::generateMicrosurfaceNormal(const math::Vector &normal, const math::Vector &incident, const math::Vector &u, const math::Vector &v) const {
	math::real r1 = math::uniformRandom();
	math::real r2 = math::uniformRandom();

	math::real rho_m = 2 * math::constants::PI * r2;
	math::real cos_theta_m = ::pow(r1, (math::real)1.0 / (m_power + (math::real)2.0));
	math::real sin_theta_m = ::sqrt((math::real)1.0 - cos_theta_m * cos_theta_m);

	math::real x = sin_theta_m * ::cos(rho_m);
	math::real y = sin_theta_m * ::sin(rho_m);
	math::real z = cos_theta_m;

	return math::add(math::add(
		math::mul(math::loadScalar(x), u),
		math::mul(math::loadScalar(y), v)),
		math::mul(math::loadScalar(z), normal));
}

manta::math::real manta::PhongBSDF::generateWeight(const math::Vector &normal, const math::Vector &incident, const math::Vector &m, const math::Vector &o) const {
	math::real g = bidirectionalShadowMasking(normal, incident, o, m);
	math::real i_dot_m = math::getScalar(math::dot(incident, m));
	math::real i_dot_n = math::getScalar(math::dot(incident, normal));
	math::real m_dot_n = math::getScalar(math::dot(m, normal));

	math::real num = ::abs(i_dot_m) * g;
	math::real div = ::abs(i_dot_n) * ::abs(m_dot_n);

	if (div < 1E-6) {
		if (num < 1E-6) return (math::real)1.0;
		else return (math::real)0.0;
	}

	return num / div;
}

manta::math::real manta::PhongBSDF::g1(const math::Vector &normal, const math::Vector &incident, const math::Vector &v, const math::Vector &m) const {
	math::Vector m_dot_v = math::dot(m, v);
	
	if (math::getScalar(math::div(m_dot_v, math::dot(v, normal))) < 0.0) {
		return (math::real)0.0;
	}

	math::real m_dot_v_s = math::getScalar(m_dot_v);
	math::real a = ::sqrt(((math::real)0.5 * m_power + 1) / (1 - m_dot_v_s * m_dot_v_s)) * m_dot_v_s;
	a = ::abs(a);

	if (a < (math::real)0.0) {
		a = (math::real)0.0;
	}

	math::real secondTerm = (math::real)1.0;
	if (a < (math::real)1.6) {
		secondTerm = (math::real)((3.535 * a + 2.181 * a * a) / (1 + 2.767 * a + 2.577 * a * a));
	}

	return secondTerm;
}